mod app;
use app::input;
use app::Item;
use app::buy;
use std::fs;

fn main() {
    let menu = "    1. Start Purchasing
    2. Show available stock
    3. Exit";

    let content = fs::read_to_string("./items.txt")
        .expect("Could not find item list file in root directory.");
    
    let mut items = Item::parse(&content)
        .into_iter()
        .map(|t| {
            let v = Item::build(t);
            v
        })
        .collect::<Vec<Item>>();
    
    println!("Welcome to MYSTORE:");
    loop {
        match input(format!("{}\nPlease choose an option", menu).as_str()).as_str() {
            "3" => break,
            "2" => {
                println!("Listing all items");
                items.iter().for_each(|x| {
                    println!("{}", x);
                });
                input("Press any key to return to menu");
                continue;
            },
            "1" => {
                let c = buy(&items);
                println!("Calculating total amount...");
                if c.is_empty() {
                    println!("Cart is empty. Total price is 0\n");
                    continue;
                }
                let mut cost = 0.0;
                c.iter().for_each(|i| {
                    cost += items[i.item].price * i.amount as f64;
                    items[i.item].stock -= i.amount;
                });
                println!("Total price is: {cost}");
            },
            _ => {
                println!("Unknown input. Please specify one from the menu.");
                continue;
            }
        }
    }
    println!("\nShutting down process...");
}
