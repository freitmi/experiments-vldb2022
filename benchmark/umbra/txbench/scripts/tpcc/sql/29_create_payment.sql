CREATE FUNCTION payment(w_id INTEGER, warehouse_count INTEGER, generate_remote_accesses INTEGER)
AS
$$
    let d_id : INTEGER;
    let mut c_id : INTEGER = 0;
    let c_w_id : INTEGER;
    let c_d_id : INTEGER;
    let h_date : DATE;
    let h_amount : NUMERIC(6,2);

    d_id = urand(1,10);

    if (generate_remote_accesses > 0) AND (urand(1,100) > 85) {
        -- remote customer
        c_w_id = urandexcept(1, warehouse_count, w_id);
        c_d_id = urand(1, 10);
    } else {
        -- local customer
        c_w_id = w_id;
        c_d_id = d_id;
    }

    h_date = now();
    h_amount = 0.01 * urand(100, 500000);

    if urand(1,100) <= 60 {
        -- payment by name
        let c_last = genName(nurand(255, 0, 999));

        -- as requested by TPC-C, find the middle customer
        SELECT customer.c_id AS it
        FROM customer
        WHERE customer.c_last = c_last
          AND customer.c_d_id = c_d_id
          AND customer.c_w_id = c_w_id
        ORDER BY c_first ASC {
            c_id = it;
        } else {
            raise 'no customer found';
        }
    } else {
        -- payment by id
        c_id = nurand(1023, 1, 3000);
    }

    SELECT w_name, w_street_1, w_street_2, w_city, w_state, w_zip, w_ytd
    FROM warehouse
    WHERE warehouse.w_id = w_id;

    let w_new_ytd = w_ytd + h_amount;

    UPDATE warehouse
    SET w_ytd = w_new_ytd
    WHERE warehouse.w_id = w_id;

    SELECT d_name, d_street_1, d_street_2, d_city, d_state, d_zip, d_ytd
    FROM district
    WHERE district.d_id = d_id
      AND district.d_w_id = w_id;

    let d_new_ytd = d_ytd + h_amount;

    UPDATE district
    SET d_ytd = d_new_ytd
    WHERE district.d_id = d_id
      AND district.d_w_id = w_id;

    SELECT c_first, c_middle, c_last, c_street_1, c_street_2, c_city, c_state, c_zip, c_phone, c_since, c_credit, c_credit_lim, c_discount, c_balance, c_ytd_payment, c_payment_cnt
    FROM customer
    WHERE customer.c_id = c_id
      AND customer.c_d_id = c_d_id
      AND customer.c_w_id = c_w_id;

    let c_new_balance = c_balance - h_amount;
    let c_new_ytd_payment = c_ytd_payment + h_amount;
    let c_new_payment_cnt = c_payment_cnt + 1;

    if c_credit = 'BC' {
        SELECT c_data
        FROM customer
        WHERE customer.c_id = c_id
          AND customer.c_d_id = c_d_id
          AND customer.c_w_id = c_w_id;

        let c_new_data = c_data || c_id::TEXT || c_d_id::TEXT || c_w_id::TEXT || d_id::TEXT || w_id::TEXT || h_amount::TEXT || h_date::TEXT || w_name::TEXT || d_name::TEXT;

        UPDATE customer
        SET c_balance = c_new_balance,
            c_ytd_payment = c_new_ytd_payment,
            c_payment_cnt = c_new_payment_cnt,
            c_data = substring(c_new_data from 1 for 500)
        WHERE customer.c_id = c_id
          AND customer.c_d_id = c_d_id
          AND customer.c_w_id = c_w_id;
    } else {
        UPDATE customer
        SET c_balance = c_new_balance,
            c_ytd_payment = c_new_ytd_payment,
            c_payment_cnt = c_new_payment_cnt
        WHERE customer.c_id = c_id
          AND customer.c_d_id = c_d_id
          AND customer.c_w_id = c_w_id;
    }

    INSERT INTO history (h_c_id, h_c_d_id, h_c_w_id, h_d_id, h_w_id, h_date, h_amount, h_data)
    VALUES (c_id, c_d_id, c_w_id, d_id, w_id, h_date, h_amount, w_name || ' ' || d_name);

    COMMIT;
$$ LANGUAGE 'umbrascript' STRICT;
