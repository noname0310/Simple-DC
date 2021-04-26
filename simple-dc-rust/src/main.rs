use std::io;
use crate::simple_dc::{DcCalculator, Token};

fn main() {
    let mut dc_calculator = DcCalculator::new();
    loop {
        let mut input_text = String::new();
        io::stdin()
            .read_line(&mut input_text)
            .expect("failed to read from stdin");

        let input_text: Vec<&str> = input_text.split(' ').collect();
        for item in input_text {
            if item.is_empty() {
                continue;
            }
            let result = Token::lex_single(item);
            let result = dc_calculator.step(result);

            match result {
                Ok(result) => {
                    match result {
                        Some(result) => {
                            println!("Ok: {}", result);
                            if result == "q" {
                                std::process::exit(0);
                            }
                        },
                        None => break,
                    }
                },
                Err(err) => {
                    match err {
                        Some(err) => println!("Err: {}", err),
                        None => println!("Err"),
                    }
                }
            }
        }
    }
}

mod simple_dc {
    pub struct DcCalculator {
        stack: Vec<i32>,
    }

    impl DcCalculator {
        pub fn new() -> DcCalculator {
            DcCalculator { stack: Vec::new() }
        }

        pub fn step(&mut self, token: Token) -> Result<Option<&'static str>, Option<&'static str>> {
            match token {
                Token::Add => {
                    let result = self.check_stack_size();
                    match result {
                        Ok(..) => {
                            let value = self.stack.pop().unwrap() + self.stack.pop().unwrap();
                            self.stack.push(value);
                            Ok(None)
                        },
                        Err(..) => result,
                    }
                },
                Token::Sub => {
                    let result = self.check_stack_size();
                    match result {
                        Ok(..) => {
                            let value = self.stack.pop().unwrap() - self.stack.pop().unwrap();
                            self.stack.push(value);
                            Ok(None)
                        },
                        Err(..) => result,
                    }
                },
                Token::Mul => {
                    let result = self.check_stack_size();
                    match result {
                        Ok(..) => {
                            let value = self.stack.pop().unwrap() * self.stack.pop().unwrap();
                            self.stack.push(value);
                            Ok(None)
                        },
                        Err(..) => result,
                    }
                },
                Token::Div => {
                    let result = self.check_stack_size();
                    match result {
                        Ok(..) => {
                            let value = self.stack.pop().unwrap() / self.stack.pop().unwrap();
                            self.stack.push(value);
                            Ok(None)
                        },
                        Err(..) => result,
                    }
                },
                Token::Print => {
                    if 0 < self.stack.len() {
                        Ok(Some(self.stack.last().unwrap().to_string().as_str()))
                    } else {
                        Ok(Some("EOF"))
                    }
                },
                Token::Quit => Ok(Some("q")),
                Token::Value(token) => {
                    match token.parse::<i32>() {
                        Ok(result) => {
                            self.stack.push(result);
                            Ok(None)
                        },
                        Err(..) => Err(Some("Invalid token")),
                    }
                },
            }
        }

        fn check_stack_size(&self) -> Result<Option<&'static str>, Option<&'static str>> {
            if self.stack.len() < 2 {
                Err(Some("stack.Count < 2"))
            } else {
                Ok(None)
            }
        }
    }

    pub enum Token<'a> {
        Add,
        Sub,
        Mul,
        Div,
        Print,
        Quit,
        Value(&'a str)
    }

    impl Token<'_> {
        pub fn lex_single(str: &str) -> Token {
            let str = str.trim();
            match str {
                "+" => Token::Add,
                "-" => Token::Sub,
                "*" => Token::Mul,
                "/" => Token::Div,
                "p" => Token::Print,
                "q" => Token::Quit,
                _ => Token::Value(str),
            }
        }
    }
}