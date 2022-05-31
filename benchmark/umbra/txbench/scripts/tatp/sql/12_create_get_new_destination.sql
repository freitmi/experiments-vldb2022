CREATE FUNCTION get_new_destination(subscriber_count INTEGER)
AS
$$
    let s_id : INTEGER;
    let sf_type : SMALLINT;
    let start_time : SMALLINT;
    let end_time : SMALLINT;

    s_id = urand(1, subscriber_count);
    sf_type = CAST (urand(1, 4) AS SMALLINT);
    start_time = CAST (8 * urand(0, 2) AS SMALLINT);
    end_time = CAST (urand(1, 24) AS SMALLINT);

    SELECT special_facility.s_id AS it
      FROM special_facility
     WHERE special_facility.s_id = s_id
       AND special_facility.sf_type = sf_type
       AND special_facility.is_active = 1 {

       SELECT call_forwarding.numberx
         FROM call_forwarding
        WHERE call_forwarding.s_id = s_id
          AND call_forwarding.sf_type = sf_type
          AND call_forwarding.start_time <= start_time
          AND call_forwarding.end_time > end_time {

         -- do nothing, we just have to select data
       }
    }

    COMMIT;
$$ LANGUAGE 'umbrascript' STRICT;