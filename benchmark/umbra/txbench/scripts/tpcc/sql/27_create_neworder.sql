CREATE FUNCTION newOrder(w_id INTEGER, warehouse_count INTEGER, generate_remote_accesses INTEGER,
                         generate_rollbacks INTEGER)
AS
$$
    let d_id : INTEGER;
    let c_id : INTEGER;
    let o_ol_cnt : INTEGER;
    let mut o_all_local : INTEGER = 1;
    let should_rollback : BOOL;

    d_id = urand(1, 10);
    c_id = nurand(1023, 1, 3000);
    o_ol_cnt = urand(5, 15);
    should_rollback = (generate_rollbacks > 0) AND (urand(1,100) <= 1);

    CREATE TEMPORARY TABLE positions (
        ol_i_id INTEGER NOT NULL,
        ol_number INTEGER NOT NULL,
        ol_supply_w_id INTEGER NOT NULL,
        ol_quantity INTEGER NOT NULL
    );

    SELECT ol_number FROM generate_series(1, o_ol_cnt) g(ol_number) {
        let mut ol_i_id : INTEGER = 0;
        let mut ol_supply_w_id : INTEGER = w_id;
        let ol_quantity : INTEGER = urand(1, 10);

        if (ol_number = o_ol_cnt) AND should_rollback {
            ol_i_id = 100001;
        } else {
            ol_i_id = nurand(8191, 1, 100000);
        }

        if (generate_remote_accesses > 0) AND (urand(1, 100) <= 1) {
            ol_supply_w_id = urandexcept(1, warehouse_count, w_id);
            o_all_local = 0;
        }

        INSERT INTO positions (ol_i_id, ol_number, ol_supply_w_id, ol_quantity)
        VALUES (ol_i_id, ol_number, ol_supply_w_id, ol_quantity);
    }

    SELECT w_tax
    FROM warehouse
    WHERE warehouse.w_id = w_id;

    SELECT d_next_o_id, d_tax
    FROM district
    WHERE district.d_id = d_id
      AND district.d_w_id = w_id;

    SELECT c_discount, c_last, c_credit
    FROM customer
    WHERE customer.c_id = c_id
      AND customer.c_d_id = d_id
      AND customer.c_w_id = w_id;

    let new_d_next_o_id = d_next_o_id + 1;

    UPDATE district
    SET d_next_o_id = new_d_next_o_id
    WHERE district.d_id = d_id
      AND district.d_w_id = w_id;

    INSERT INTO orders (o_id, o_d_id, o_w_id, o_c_id, o_entry_d, o_ol_cnt, o_all_local)
    VALUES (d_next_o_id, d_id, w_id, c_id, NOW(), o_ol_cnt, o_all_local);

    INSERT INTO new_orders (no_o_id, no_d_id, no_w_id)
    VALUES (d_next_o_id, d_id, w_id);

    SELECT * FROM positions {
        SELECT i_price
          FROM item
         WHERE item.i_id = ol_i_id else {
            continue;
        }

        SELECT s_quantity,
               CASE d_id
                    WHEN 1 THEN s_dist_01
                    WHEN 2 THEN s_dist_02
                    WHEN 3 THEN s_dist_03
                    WHEN 4 THEN s_dist_04
                    WHEN 5 THEN s_dist_05
                    WHEN 6 THEN s_dist_06
                    WHEN 7 THEN s_dist_07
                    WHEN 8 THEN s_dist_08
                    WHEN 9 THEN s_dist_09
                    WHEN 10 THEN s_dist_10
               END AS s_dist,
               s_ytd, s_order_cnt, s_remote_cnt
        FROM stock
        WHERE stock.s_w_id = ol_supply_w_id
          AND stock.s_i_id = ol_i_id;

        let s_new_quantity = CASE WHEN s_quantity >= ol_quantity + 10 then s_quantity - ol_quantity else s_quantity + 91 - ol_quantity END;
        let s_new_remote_cnt = s_remote_cnt + CASE WHEN ol_supply_w_id <> w_id THEN 1 ELSE 0 END;
        let s_new_order_cnt = s_order_cnt + 1;
        let s_new_ytd = s_ytd + ol_quantity;

        UPDATE stock
        SET s_quantity = s_new_quantity,
            s_remote_cnt = s_new_remote_cnt,
            s_order_cnt = s_new_order_cnt,
            s_ytd = s_new_ytd
        WHERE stock.s_w_id = ol_supply_w_id
          AND stock.s_i_id = ol_i_id;

        INSERT INTO order_line (ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id, ol_supply_w_id, ol_quantity, ol_amount, ol_dist_info)
        VALUES (d_next_o_id, d_id, w_id, ol_number, ol_i_id, ol_supply_w_id, ol_quantity, ol_quantity * i_price, s_dist);
    }

    if should_rollback {
        ROLLBACK;
    } else {
        COMMIT;
    }
$$ LANGUAGE 'umbrascript' STRICT;
