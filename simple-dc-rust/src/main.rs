use std::io;

fn main() {
    let vec = Vec::new();

    let mut input_text = String::new();
    loop {
        io::stdin()
            .read_line(&mut input_text)
            .expect("failed to read from stdin");
    }
    let input_text = input_text.trim();

    match trimmed.parse::<u32>() {
        Ok(i) => println!("your integer input: {}", i),
        Err(..) => println!("this was not an integer: {}", trimmed),
    };
}
