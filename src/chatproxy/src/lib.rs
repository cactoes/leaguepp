#![allow(non_snake_case)]
#![no_main]

use async_std::net::{ TcpListener, TcpStream };
use async_native_tls::{ TlsAcceptor, TlsStream, TlsConnector };
use async_std::prelude::*;
use std::sync::atomic::{ AtomicBool, Ordering };

static KEEP_ALIVE: AtomicBool = AtomicBool::new(false);
static USE_PROXY: AtomicBool = AtomicBool::new(true);

const PFX_FILE_DATA: &[u8] = include_bytes!("../server.pfx");

#[no_mangle]
extern "system" fn ChatProxyStartServer(remote: *const u8, remotePort: u32, localPort: u32) -> bool {
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
                handle_client(&mut client_stream, &mut target_stream).await.unwrap();
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

async fn handle_client(client_stream: &mut TlsStream<TcpStream>, target_stream: &mut TlsStream<TcpStream>) -> async_std::io::Result<()> {
    let mut client_buffer = [0; 1024 * 8];
    let mut target_buffer = [0; 1024 * 8];
    
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

                if USE_PROXY.load(Ordering::Relaxed) {
                    let data_string = String::from_utf8_lossy(data);
                    let data_string = data_string.replace("chat", "offline");
                    let data_string = data_string.replace("status", "unused");
                    target_stream.write_all(data_string.as_bytes()).await.unwrap();
                } else {
                    target_stream.write_all(data).await.unwrap();
                }

                client_buffer = [0; 1024 * 8];
            }
            futures::future::Either::Right((Ok(0), _)) => { client_stream.write_all(&[]).await.unwrap(); }
            futures::future::Either::Right((Err(_e), _)) => { break; }
            futures::future::Either::Right((Ok(bytes_read), _)) => {
                client_stream.write_all(&target_buffer[..bytes_read]).await.unwrap();
                target_buffer = [0; 1024 * 8];
            }
        }
    }

    return Ok(());
}