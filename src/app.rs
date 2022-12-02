use std::fmt;
use std::io;

/// Take input from users thro stdin
pub fn input(query: &str) -> String {
    if !query.is_empty() {
        println!("{query}");
    }
    let mut res = String::new();
    io::stdin().read_line(&mut res).unwrap();

    // Trim it for any whitespaces and return a newly generated String
    String::from(res.trim())
}

/// An item struct
#[derive(Debug)]
pub struct Item {
    pub id: String,
    pub name: String,
    pub price: f64,
    pub stock: i64,
}

impl Item {
    /// Parse string to separate lines into a vec
    pub fn parse(contents: &str) -> Vec<&str> {
        let mut res = Vec::new();
        for line in contents.lines() {
            res.push(line);
        }
        res
    }
    /// Create item structs
    pub fn build(string: &str) -> Self {
        let mut iter = string.split_whitespace();
        let id = iter.next().unwrap_or("0");
        let name = iter.next().unwrap_or("0");
        let price: f64 = iter.next().unwrap_or("0").parse().unwrap_or(0.0);
        let stock: i64 = iter.next().unwrap_or("0").parse().unwrap_or(0);
        Item {
            id: id.to_string(),
            name: name.to_string(),
            price,
            stock,
        }
    }
}

impl fmt::Display for Item {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "Name: {}, ID: {}, Price: {}, Stock: {}",
            self.name, self.id, self.price, self.stock
        )
    }
}

#[derive(Debug)]
pub struct Cart {
    pub item: usize,
    pub amount: i64,
}

impl  fmt::Display for Cart {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "Item index: {}, amount: {}",
            self.item, self.amount
        )
    }
}

pub fn buy(v: &Vec<Item>) -> Vec<Cart> {
    let mut cart: Vec<Cart> = Vec::new();
    loop {
        let code = input("Provide an item id to add to cart").to_lowercase();
        if code == "-1" {
            break;
        }
        let index = match v.iter().position(|x| x.id.to_lowercase() == code) {
            Some(n) => n,
            None => {
                println!("Invalid item code provided. Please provide a valid one.");
                continue;
            }
        };
        let item = &v[index];
        if item.stock == 0 {
            println!("This item is currently out of stock. Please try a different one");
            continue;
        }
        println!("Selected item {}. Available stock {} \nPlease provide the amount: ", item.name, item.stock);
        let amount = loop {
            let am: i64 = match input("").parse() {
                Ok(v) => v,
                Err(_) => {
                    println!("Invalid input provided. Please provide a valid positive integer amount");
                    continue;
                }
            };
            if am > item.stock {
                println!("Amount {am} is greater than {}. Please provide a value equal or less than stock.", item.stock);
                continue;
            }
            if am <= 0 {
                println!("Amount cannot be less than 1. Must be at least or more than 1. Please try again");
                continue;
            }
            break am;
        };
        cart.push(Cart {
            item: index,
            amount,
        })
    }
    cart
}
