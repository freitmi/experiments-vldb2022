CREATE FUNCTION update_location(subscriber_count INTEGER)
AS
$$
    let sub_nbr : TEXT;
    let vlr_location : INTEGER;

    sub_nbr = subrand(subscriber_count);
    vlr_location = urand(1, 2147483647);

    UPDATE subscriber
       SET vlr_location = vlr_location
     WHERE subscriber.sub_nbr = sub_nbr;

    COMMIT;
$$ LANGUAGE 'umbrascript' STRICT;