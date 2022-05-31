CREATE FUNCTION update_subscriber_data(subscriber_count INTEGER)
AS
$$
    let s_id : INTEGER;
    let sf_type : SMALLINT;
    let bit_1 : SMALLINT;
    let data_a : SMALLINT;
    let mut should_rollback : BOOLEAN = false;

    s_id = urand(1, subscriber_count);
    sf_type = CAST (urand(1, 4) AS SMALLINT);
    bit_1 = CAST (urand(0, 1) AS SMALLINT);
    data_a = CAST (urand(0, 255) AS SMALLINT);

    UPDATE subscriber
       SET bit_1 = bit_1
     WHERE subscriber.s_id = s_id;

    UPDATE special_facility
       SET data_a = data_a
     WHERE special_facility.s_id = s_id
       AND special_facility.sf_type = sf_type
    RETURNING 1 AS it else {
        should_rollback = true;
    }

    if should_rollback {
        ROLLBACK;
    } else {
        COMMIT;
    }
$$ LANGUAGE 'umbrascript' STRICT;