fn encrypt(pt: &mut str, key: &str) {
    let key = key.as_bytes();

    unsafe {
        let pt = pt.as_bytes_mut();

        for i in 0..pt.len() {
            pt[i] = pt[i].wrapping_add(key[i % key.len()]);
        }
    };
}

fn decrypt(ct: &mut str, key: &str) {
    let key = key.as_bytes();

    unsafe {
        let ct = ct.as_bytes_mut();

        for i in 0..ct.len() {
            ct[i] = ct[i].wrapping_sub(key[i % key.len()]);
        }
    };
}

fn main() {}
