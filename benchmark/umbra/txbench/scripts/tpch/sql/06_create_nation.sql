create table nation
(
    n_nationkey integer      not null,
    n_name      char(25)     not null,
    n_regionkey integer      not null,
    n_comment   varchar(152) not null,
    primary key (n_nationkey)
) with (storage = paged);
