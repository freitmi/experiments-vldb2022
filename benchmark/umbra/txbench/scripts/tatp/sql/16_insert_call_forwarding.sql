CREATE FUNCTION insert_call_forwarding(subscriber_count INTEGER)
AS
$$
    let sub_nbr : TEXT;
    let sf_type : SMALLINT;
    let start_time : SMALLINT;
    let end_time : SMALLINT;
    let numberx : TEXT;

    let mut success : BOOLEAN = true;

    sub_nbr = subrand(subscriber_count);
    sf_type = CAST (urand(1, 4) AS SMALLINT);
    start_time = CAST (8 * urand(0, 2) AS SMALLINT);
    end_time = CAST (urand(1, 24) AS SMALLINT);
    numberx = subrand(subscriber_count);

    SELECT s_id
      FROM subscriber
     WHERE subscriber.sub_nbr = sub_nbr;

    SELECT sf_type AS existing_sf_type,
           start_time AS existing_start_time
      FROM special_facility
     WHERE special_facility.s_id = s_id {
        if (sf_type = existing_sf_type) AND (start_time = existing_start_time) {
            success = false;
        }
     }

    if success {
        INSERT INTO call_forwarding(s_id, sf_type, start_time, end_time, numberx)
        VALUES (s_id, sf_type, start_time, end_time, numberx);
    }

    COMMIT;
$$ LANGUAGE 'umbrascript' STRICT;