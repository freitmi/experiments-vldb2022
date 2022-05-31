CREATE FUNCTION get_access_data(subscriber_count INTEGER)
AS
$$
    let s_id : INTEGER;
    let ai_type : SMALLINT;

    s_id = urand(1, subscriber_count);
    ai_type = CAST (urand(1, 4) AS SMALLINT);

    SELECT access_info.data1, access_info.data2, access_info.data3, access_info.data4
      FROM access_info
     WHERE access_info.s_id = s_id
       AND access_info.ai_type = ai_type {

     -- do nothing, we just have to select data
   }

    COMMIT;
$$ LANGUAGE 'umbrascript' STRICT;