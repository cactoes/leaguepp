declare async function invoke<Ty>(functionName: string,
    args: (string | number | boolean | (string | number | boolean)[])[]): Promise<Ty>;
    
declare function register<Ty extends (string | number | boolean | (string | number | boolean)[])[]>(
    callback: (...args: Ty) => void): void;