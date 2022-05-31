CREATE TABLE history
(
    h_c_id   INTEGER,
    h_c_d_id INTEGER,
    h_c_w_id INTEGER,
    h_d_id   INTEGER,
    h_w_id   INTEGER,
    h_date   DATE,
    h_amount DECIMAL(6, 2),
    h_data   VARCHAR(24)
) PARTITION BY HASH (h_w_id) WITH (STORAGE = paged);