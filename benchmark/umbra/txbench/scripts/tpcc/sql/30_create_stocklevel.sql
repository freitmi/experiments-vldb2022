CREATE FUNCTION stockLevel(w_id INTEGER)
AS
$$
    let d_id : INTEGER;
    let threshold : INTEGER;

    d_id = urand(1, 10);
    threshold = urand(10, 20);

    SELECT d_next_o_id
    FROM district
    WHERE district.d_id = d_id
      AND district.d_w_id = w_id;

    -- FIXME: ol_i_id should be deduplicated but our group by is slow on small inputs
    SELECT ol_i_id
    FROM order_line
    WHERE order_line.ol_d_id = d_id
      AND order_line.ol_w_id = w_id
      AND order_line.ol_o_id >= d_next_o_id - 20 {

        SELECT CASE WHEN s_quantity < threshold THEN 1 ELSE 0 END
        FROM stock
        WHERE stock.s_w_id = w_id
          AND stock.s_i_id = ol_i_id;
    }

    COMMIT;
$$ LANGUAGE 'umbrascript' STRICT;
