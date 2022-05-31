CREATE FUNCTION delivery(w_id INTEGER)
AS
$$
    let o_new_carrier_id : INTEGER;
    let ol_new_delivery_d : DATE;

    o_new_carrier_id = urand(1, 10);
    ol_new_delivery_d = now();

    SELECT d_id FROM generate_series(1, 10) g(d_id) {
        SELECT no_o_id
        FROM new_orders
        WHERE new_orders.no_w_id = w_id
          AND new_orders.no_d_id = CAST(d_id AS INTEGER)
        ORDER BY no_o_id ASC
        LIMIT 1 else {
            continue;
        }

        DELETE
        FROM new_orders
        WHERE new_orders.no_o_id = no_o_id
          AND new_orders.no_d_id = CAST (d_id AS INTEGER)
          AND new_orders.no_w_id = w_id;

        SELECT o_c_id
        FROM orders
        WHERE orders.o_id = no_o_id
          AND orders.o_d_id = CAST (d_id AS INTEGER)
          AND orders.o_w_id = w_id;

        UPDATE orders
        SET o_carrier_id = o_new_carrier_id
        WHERE orders.o_id = no_o_id
          AND orders.o_d_id = CAST (d_id AS INTEGER)
          AND orders.o_w_id = w_id;

        let mut ol_total : NUMERIC(6, 2) = 0;

        SELECT ol_amount
        FROM order_line
        WHERE order_line.ol_o_id = no_o_id
          AND order_line.ol_d_id = CAST (d_id AS INTEGER)
          AND order_line.ol_w_id = w_id {
            ol_total = ol_total + ol_amount;
        }

        UPDATE order_line
        SET ol_delivery_d = ol_new_delivery_d
        WHERE order_line.ol_o_id = no_o_id
          AND order_line.ol_d_id = CAST (d_id AS INTEGER)
          AND order_line.ol_w_id = w_id;

        UPDATE customer
        SET c_balance = c_balance + ol_total,
            c_delivery_cnt = c_delivery_cnt + 1
        WHERE customer.c_id = o_c_id
          AND customer.c_d_id = CAST (d_id AS INTEGER)
          AND customer.c_w_id = w_id;
    }

    COMMIT;
$$ LANGUAGE 'umbrascript' STRICT;
