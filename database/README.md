# Database Concepts

## Transactions

A database transaction is a unit of work performed within a database management system against a database. Transactions are coherrent and reliable, independent of other transactions. A transaction generally represents any change in the database. It has 2 main purposes -

- Provide reliable unit of work that recovers the database from any kind of system failure and keeps the database consistent.
- Provide isolation between multiple transactions happening on the database to avoid error.

A transaction is often made of multiple operations. A database transaction must follow the ACID principle. ACID has 4 words -

- atomic: it must be either complete on it's entirity or have no effects whatsoever
- consistent: it must follow the existing database constraints
- isolated: it must not effect other transactions
- durable: it must save to persistent storage

Example: In a double entry accounting system every debit requires a corresponding credit. If one buys groceries with $100 then someone else will receive that $100 in their account. So it has the following 2 operations which make up the transaction: 

1. Debit $100 to grocery expense account
2. Credit $100 to checking account


