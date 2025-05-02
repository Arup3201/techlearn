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

Example: To transfer $50 from account A to account B we need to do the following set of operations:

1. read(A)
2. A := A - 50
3. write(A)
4. read(B)
5. B := B + 50
6. write(B)

There are 2 main issues while transactions happen -

- Failures of various kinds, such as hardware, software etc.
- Concurrent execution of multiple transactions

Lets try to understand the ACID property through the example -

1. Atomicity: If the transaction fails after step 3 then the money will be "lost". Maybe due to some software failures. It will lead to an inconsistent database state. The system should ensure that the updates of a partially executed transaction will not be reflected on the database.
2. Durability: Once the user has been notified that the transactions has been completed, the updates to the database after the transaction must persist even if there are any software or hardware failures.
3. Consistency: The sum of A and B should be unchanged by the execution of the transaction. Consistency requires specified integrity of the database constraints such as primary key and foreign keys. During transaction execution the database might be temporarily inconsistent. After the transaction, the database must be consistent.

