#![allow(non_snake_case)]
#![no_main]

use async_std::net::{ TcpListener, TcpStream };
use async_native_tls::{ TlsAcceptor, TlsStream, TlsConnector };
use async_std::prelude::*;
use std::{sync::atomic::{ AtomicBool, Ordering }, time::SystemTime};

static KEEP_ALIVE: AtomicBool = AtomicBool::new(false);
static USE_PROXY: AtomicBool = AtomicBool::new(true);

const PFX_FILE_DATA: &[u8] = include_bytes!("../server.pfx");
const READ_BUFFER_SIZE: usize = 1024 * 16;

#[no_mangle]
extern "system" fn ChatProxyStartServer(remote: *const u8, remotePort: u32, localPort: u32, fp: extern "C" fn(bool)) -> bool {
    if KEEP_ALIVE.load(Ordering::Relaxed) { 
        return false;
    }

    KEEP_ALIVE.store(true, Ordering::Relaxed);

    let remote_string = unsafe { std::ffi::CStr::from_ptr(remote as *const i8).to_str().unwrap() };

    let task_handle = async_std::task::spawn(async move {
        let acceptor = TlsAcceptor::new(PFX_FILE_DATA, "").await.unwrap();
        let listener = TcpListener::bind(format!("127.0.0.1:{}", localPort)).await.unwrap();

        while let Ok((stream, _)) = listener.accept().await {
            if !KEEP_ALIVE.load(Ordering::Relaxed) {
                break;
            }

            let acceptor = acceptor.clone();
            async_std::task::spawn(async move {
                let mut client_stream = acceptor.accept(stream).await.unwrap();
                let mut target_stream = create_connection(remote_string, remotePort).await;
                handle_client(&mut client_stream, &mut target_stream, fp).await.unwrap();
                fp(false);
            });
        }
    });

    async_std::task::block_on(task_handle);
    return true;
}

#[no_mangle]
extern "system" fn ChatProxyStopServer() -> bool {
    if !KEEP_ALIVE.load(Ordering::Relaxed) {
        return false;
    }

    KEEP_ALIVE.store(false, Ordering::Relaxed);
    return true;
}

#[no_mangle]
extern "system" fn ChatProxyUseProxy(state: bool) {
    USE_PROXY.store(state, Ordering::Relaxed);
}

async fn create_connection(url: &str, port: u32) -> TlsStream<TcpStream> {
    return TlsConnector::new()
        .connect(url, TcpStream::connect(format!("{}:{}", url, port)).await.unwrap())
        .await
        .unwrap();
}

fn set_status(data: String) -> String {
    if !USE_PROXY.load(Ordering::Relaxed) {
        return data;
    }

    return data
        .replace("chat", "offline")
        .replace("away", "offline")
        .replace("status", "unused");
}

fn get_presence(bio: &str, profile_customization: &str) -> String {
    let now = SystemTime::now().duration_since(SystemTime::UNIX_EPOCH).unwrap().as_secs();
    return format!(
        "<presence from='41c322a1-b328-495b-a004-5ccd3e45eae8@eu1.pvp.net/RC-lpp' id='presence_88'>
            <games>
                <keystone>
                    <st>chat</st>
                    <s.t>{}</s.t>
                    <m>{}</m>
                    <s.p>keystone</s.p>
                    <pty/>
                </keystone>
                <league_of_legends>
                    <st>chat</st>
                    <m>{}</m>
                    <p>{}</p>
                    <s.p>league_of_legends</s.p>
                    <s.t>{}</s.t>
                    <pty/>
                </league_of_legends>
            </games>
            <platform>windows</platform>
            <show>chat</show>
            <status/>
        </presence>", now, bio, bio, profile_customization, now);
}

async fn handle_client(client_stream: &mut TlsStream<TcpStream>, target_stream: &mut TlsStream<TcpStream>, fp: extern "C" fn(bool)) -> async_std::io::Result<()> {
    let mut client_buffer = [0; READ_BUFFER_SIZE];
    let mut target_buffer = [0; READ_BUFFER_SIZE];

    while KEEP_ALIVE.load(Ordering::Relaxed) {
        let client_read = client_stream.read(&mut client_buffer);
        let target_read = target_stream.read(&mut target_buffer);

        futures::pin_mut!(client_read);
        futures::pin_mut!(target_read);

        match futures::future::select(client_read, target_read).await {
            futures::future::Either::Left((Ok(0), _)) => { target_stream.write_all(&[]).await.unwrap(); }
            futures::future::Either::Left((Err(_e), _)) => { break; }
            futures::future::Either::Left((Ok(bytes_read), _)) => {
                let data = &client_buffer[..bytes_read];
                target_stream.write_all(set_status(String::from_utf8_lossy(data).to_string()).as_bytes()).await.unwrap();
                client_buffer = [0; READ_BUFFER_SIZE];

                fp(USE_PROXY.load(Ordering::Relaxed));
            }
            futures::future::Either::Right((Ok(0), _)) => { client_stream.write_all(&[]).await.unwrap(); }
            futures::future::Either::Right((Err(_e), _)) => { break; }
            futures::future::Either::Right((Ok(bytes_read), _)) => {
                let data_string = String::from_utf8_lossy(&target_buffer[..bytes_read]);
                
                if data_string.contains("<query xmlns='jabber:iq:riotgames:roster'>") {
                    let data_string  = data_string.replace(
                        "<query xmlns='jabber:iq:riotgames:roster'>",
                        "<query xmlns='jabber:iq:riotgames:roster'>
                        <item jid='41c322a1-b328-495b-a004-5ccd3e45eae8@eu1.pvp.net' name='league++' subscription='both' puuid='41c322a1-b328-495b-a004-5ccd3e45eae8'>
                            <group priority='9999'>league++</group>
                            <id name='league++' tagline='L++'/>
                            <platforms>
                                <riot name='league++' tagline='L++'/>
                            </platforms>
                            <lol name='league++'/>
                            <state>online</state>
                        </item>");
                    client_stream.write_all(data_string.as_bytes()).await.unwrap();

                    let p = get_presence("loaded!", "{&quot;profileIcon&quot;:&quot;4658&quot;,&quot;puuid&quot;:&quot;41c322a1-b328-495b-a004-5ccd3e45eae8&quot;}");
                    client_stream.write_all(p.as_bytes()).await.unwrap();
                } else {
                    client_stream.write_all(data_string.as_bytes()).await.unwrap();
                }

                target_buffer = [0; READ_BUFFER_SIZE];
            }
        }
    }

    return Ok(());
}