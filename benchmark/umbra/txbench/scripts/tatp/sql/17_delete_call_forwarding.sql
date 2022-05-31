CREATE FUNCTION delete_call_forwarding(subscriber_count INTEGER)
AS
$$
    let sub_nbr : TEXT;
    let sf_type : SMALLINT;
    let start_time : SMALLINT;

    sub_nbr = subrand(subscriber_count);
    sf_type = CAST (urand(1, 4) AS SMALLINT);
    start_time = CAST (8 * urand(0, 2) AS SMALLINT);

    SELECT s_id
      FROM subscriber
     WHERE subscriber.sub_nbr = sub_nbr;

    DELETE
      FROM call_forwarding
     WHERE call_forwarding.s_id = s_id
       AND call_forwarding.sf_type = sf_type
       AND call_forwarding.start_time = start_time {
        -- do nothing
    }

    COMMIT;
$$ LANGUAGE 'umbrascript' STRICT;