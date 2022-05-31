CREATE FUNCTION urandexcept(lower INTEGER, upper INTEGER, v INTEGER) RETURNS INTEGER
AS
$$
    if upper <= lower {
        return lower;
    } else {
        let r = random(upper - lower) + lower;
        return case when r >= v then r + 1 else r end;
    }
$$ LANGUAGE 'umbrascript' STRICT
                          STABLE;
