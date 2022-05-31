CREATE FUNCTION orderStatus(w_id INTEGER)
AS
$$
    let mut c_id : INTEGER = 0;
    let d_id : INTEGER;

    d_id = urand(1,10);

    if urand(1,100) <= 60 {
        -- order status by name
        let c_last = genName(nurand(255, 0, 999));

        -- as requested by TPC-C, find the middle customer
        SELECT customer.c_id AS it
        FROM customer
        WHERE customer.c_last = c_last
          AND customer.c_d_id = d_id
          AND customer.c_w_id = w_id
        ORDER BY c_first ASC {
            c_id = it;
        } else {
            raise 'no customer found';
        }
    } else {
        -- order status by id
        c_id = nurand(1023, 1, 3000);
    }

    SELECT c_first, c_middle, c_last, c_balance
    FROM customer
    WHERE customer.c_id = c_id
      AND customer.c_d_id = d_id
      AND customer.c_w_id = w_id;

    SELECT o_id,
           o_entry_d,
           o_carrier_id
    FROM orders
    WHERE orders.o_c_id = c_id
      AND orders.o_d_id = d_id
      AND orders.o_w_id = w_id
    ORDER BY o_id ASC
    LIMIT 1;

    SELECT ol_i_id, ol_supply_w_id, ol_quantity, ol_amount, ol_delivery_d
    FROM order_line
    WHERE order_line.ol_o_id = o_id
      AND order_line.ol_d_id = d_id
      AND order_line.ol_w_id = w_id {
      -- do nothing, we just have to retrieve data
    }

    COMMIT;
$$ LANGUAGE 'umbrascript' STRICT;
