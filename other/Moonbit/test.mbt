struct Lexer {
    input: String,  // 输入的源代码
    position: usize, // 当前读取位置
    current_char: Option<char>, // 当前字符
}


// 初始化词法分析器
fn new(input: String) -> Self {
        let mut lexer = Lexer {
            input,
            position: 0,
            current_char: None,
        };
lexer.advance(); // 获取第一个字符
lexer
}

    // 前进到下一个字符
fn advance(&mut self) {
    if self.position < self.input.len() {
        self.current_char = self.input[self.position..].chars().next();
        self.position += 1;
    } else {
        self.current_char = None; // 输入结束
    }
}

    // 跳过空白字符
    fn skip_whitespace(&mut self) {
        while let Some(c) = self.current_char {
            if c.is_whitespace() {
                self.advance();
            } else {
                break;
            }
        }
    }

    // 获取下一个Token
    fn next_token(&mut self) -> Token {
        self.skip_whitespace();

        match self.current_char {
            Some('+') => { self.advance(); Token::PLUS }
            Some('-') => { self.advance(); Token::MINUS }
            Some('*') => { self.advance(); Token::MUL }
            Some('/') => { self.advance(); Token::DIV }
            Some('=') => {
                self.advance();
                if let Some('=') = self.current_char {
                    self.advance();
                    Token::EQ  // '=='
                } else {
                    Token::ASSIGN  // '='
                }
            }
            Some('(') => { self.advance(); Token::LPAREN }
            Some(')') => { self.advance(); Token::RPAREN }
            Some('{') => { self.advance(); Token::LBRACE }
            Some('}') => { self.advance(); Token::RBRACE }
            Some(';') => { self.advance(); Token::SEMICOLON }
            Some(',') => { self.advance(); Token::COMMA }

            // 解析数字
            Some(c) if c.is_digit(10) => self.lex_number(),
            
            // 解析标识符或关键字
            Some(c) if c.is_alphabetic() => self.lex_identifier_or_keyword(),

            // 文件结尾
            None => Token::EOF,

            _ => panic!("Unrecognized character: {:?}", self.current_char),
        }
    }

    // 解析数字
    fn lex_number(&mut self) -> Token {
        let mut num_str = String::new();

        while let Some(c) = self.current_char {
            if c.is_digit(10) || c == '.' {
                num_str.push(c);
                self.advance();
            } else {
                break;
            }
        }

        let number: f64 = num_str.parse().unwrap();
        Token::NUMBER(number)
    }

    // 解析标识符或关键字
    fn lex_identifier_or_keyword(&mut self) -> Token {
        let mut ident = String::new();

        while let Some(c) = self.current_char {
            if c.is_alphanumeric() {
                ident.push(c);
                self.advance();
            } else {
                break;
            }
        }

        match ident.as_str() {
            "true" => Token::TRUE,
            "false" => Token::FALSE,
            "if" => Token::IF,
            "else" => Token::ELSE,
            "let" => Token::LET,
            "fn" => Token::FN,
            "return" => Token::RETURN,
            _ => Token::IDENTIFIER(ident),
        }
    }
}
