# Database Concepts

**What are databases?**

Databases are collection of inter-related data that models some real world aspects. Databases are the core components of computer systems.

Example: We want to create a model for storing musics. For that we need 2 sources of data - Artists and Albums. If we keep the entries of these sources in a flat file(CSV file) which contains the entries of artists and elbums, when we want to find when GZA went solo then we have to something like the following -

```
Artists (name, yeas, country)

'GZA', 1990, 'USA'
'Wu-Tang Clan', 1992, 'USA'
'Notorious BIG', 1992, 'USA'
```

```py
for line in file.readlines():
    record = parse(line)
    if record[0]=='GZA':
        print(int(record[1]))
```

But there are lots of problems with this approach -

- Integrity: 
    - how do we ensure that the artist is same for each album entry? 
    - what if somebody overwrites the album year with invalid string?
    - what if there are multiple artists on one album?
    - what if we delete an artist that has albums?

- Implementation:
    - how do we find a particular record in the file?
    - what if two threads try to write to the same file at the same time?
    - what if we want to use a new application to access the same database? do we write the same logic in another language...

- Durability:
    - what if the system crashes in the middle of the update operation, how will the updates look like? will it be partial, complete or no change...
    - what if we want to replicate the database to another machine for higher availability?

A database management software helps you tackle these problems by managing the information stored in the database safely and securely. You will always rely on DBMS to manage your database instead of writing your own database management system which might be error prone.

A general purpose database management system supports definition, update, querying and administration of databases in accordance to dome data model.


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


