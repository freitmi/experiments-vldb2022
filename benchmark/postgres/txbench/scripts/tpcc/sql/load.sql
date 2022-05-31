COPY item FROM '/data/tpcc/item.csv' DELIMITER ',';
COPY warehouse FROM '/data/tpcc/warehouse.csv' DELIMITER ',';
COPY stock FROM '/data/tpcc/stock.csv' DELIMITER ',';
COPY district FROM '/data/tpcc/district.csv' DELIMITER ',';
COPY customer FROM '/data/tpcc/customer.csv' DELIMITER ',';
COPY history FROM '/data/tpcc/history.csv' DELIMITER ',';
COPY orders FROM '/data/tpcc/orders.csv' DELIMITER ',' NULL '';
COPY order_line FROM '/data/tpcc/order_line.csv' DELIMITER ',' NULL '';
COPY new_orders FROM '/data/tpcc/new_orders.csv' DELIMITER ',';
