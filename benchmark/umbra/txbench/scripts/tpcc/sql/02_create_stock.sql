CREATE TABLE stock
(
    s_i_id       INTEGER NOT NULL,
    s_w_id       INTEGER NOT NULL,
    s_quantity   NUMERIC(4),
    s_dist_01    CHAR(24),
    s_dist_02    CHAR(24),
    s_dist_03    CHAR(24),
    s_dist_04    CHAR(24),
    s_dist_05    CHAR(24),
    s_dist_06    CHAR(24),
    s_dist_07    CHAR(24),
    s_dist_08    CHAR(24),
    s_dist_09    CHAR(24),
    s_dist_10    CHAR(24),
    s_ytd        INTEGER,
    s_order_cnt  INTEGER,
    s_remote_cnt INTEGER,
    s_data       VARCHAR(50),
    PRIMARY KEY (s_w_id, s_i_id)
) PARTITION BY HASH (s_w_id) WITH (STORAGE = paged);
