CREATE FUNCTION genName(id INTEGER) RETURNS VARCHAR(20)
AS
$$
   return namePart(mod(id/100,10)) || namePart(mod(id/10,10)) || namePart(mod(id,10));
$$ LANGUAGE 'umbrascript' STRICT
                          IMMUTABLE;
