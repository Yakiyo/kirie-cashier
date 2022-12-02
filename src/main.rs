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
        .map(Item::build)
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
                let mut price = 0.0;
                c.iter().for_each(|i| {
                    price += items[i.item].price * i.amount as f64;
                    items[i.item].stock -= i.amount;
                    // RM2 discount implementation
                    if items[i.item].id == "A0002" && i.amount >= 4 {
                        println!("Your purchase contains more than 3 A0002 purchase. You get RM2 discount for every 4");
                        let grp = (i.amount / 4).abs();
                        price -= (grp * 2) as f64;
                    }
                });

                // Combo discount
                let b1_pos = items.iter().position(|x| x.id == "B0001").unwrap();
                let b3_pos = items.iter().position(|x| x.id == "B0003").unwrap();
                let b1_c = c.iter().find(|e| e.item == b1_pos);
                let b3_c = c.iter().find(|e| e.item == b3_pos);
                // Make sure neither of em is none, since its a combo deal
                if b1_c.is_some() && b3_c.is_some() {
                    let b1_c = b1_c.unwrap();
                    let b3_c = b3_c.unwrap();
                    let min_val = if b1_c.amount < b3_c.amount {
                        b1_c.amount
                    } else {
                        b3_c.amount
                    };
                    price -= min_val as f64;
                }

                // Net purchase discount
                if price >= 100.0 {
                    println!("Your total purchase is above 100. Applying 15% discount.");
                    price = price - ((15.0 / 100.0) * price);
                }
                println!("Total price is: {price}");
            },
            _ => {
                println!("Unknown input. Please specify one from the menu.");
                continue;
            }
        }
    }
    println!("\nShutting down process...");
}
