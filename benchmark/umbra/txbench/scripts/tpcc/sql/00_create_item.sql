CREATE TABLE item
(
    i_id    INTEGER NOT NULL,
    i_im_id INTEGER,
    i_name  VARCHAR(24),
    i_price DECIMAL(5, 2),
    i_data  VARCHAR(50),
    PRIMARY KEY (i_id)
) WITH (STORAGE = paged);
