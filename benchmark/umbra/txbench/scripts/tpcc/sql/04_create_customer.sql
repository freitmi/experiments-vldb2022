CREATE TABLE customer
(
    c_id           INTEGER NOT NULL,
    c_d_id         INTEGER NOT NULL,
    c_w_id         INTEGER NOT NULL,
    c_first        VARCHAR(16),
    c_middle       CHAR(2),
    c_last         VARCHAR(16),
    c_street_1     VARCHAR(20),
    c_street_2     VARCHAR(20),
    c_city         VARCHAR(20),
    c_state        CHAR(2),
    c_zip          CHAR(9),
    c_phone        CHAR(16),
    c_since        DATE,
    c_credit       CHAR(2),
    c_credit_lim   DECIMAL(12, 2),
    c_discount     DECIMAL(4, 4),
    c_balance      DECIMAL(12, 2),
    c_ytd_payment  DECIMAL(12, 2),
    c_payment_cnt  INTEGER,
    c_delivery_cnt INTEGER,
    c_data         TEXT,
    PRIMARY KEY (c_w_id, c_d_id, c_id)
) PARTITION BY HASH (c_w_id) WITH (STORAGE = paged);
