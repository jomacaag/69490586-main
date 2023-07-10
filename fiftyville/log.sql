-- Keep a log of any SQL queries you execute as you solve the mystery.
-- KNOWN POINTS

.tables
-- airports              crime_scene_reports   people
-- atm_transactions      flights               phone_calls
-- bakery_security_logs  interviews
-- bank_accounts         passengers


.schema
-- CREATE TABLE crime_scene_reports (
--     id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     street TEXT,
--     description TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE interviews (
--     id INTEGER,
--     name TEXT,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     transcript TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE atm_transactions (
--     id INTEGER,
--     account_number INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     atm_location TEXT,
--     transaction_type TEXT,
--     amount INTEGER,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE bank_accounts (
--     account_number INTEGER,
--     person_id INTEGER,
--     creation_year INTEGER,
--     FOREIGN KEY(person_id) REFERENCES people(id)
-- );
-- CREATE TABLE airports (
--     id INTEGER,
--     abbreviation TEXT,
--     full_name TEXT,
--     city TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE flights (
--     id INTEGER,
--     origin_airport_id INTEGER,
--     destination_airport_id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     hour INTEGER,
--     minute INTEGER,
--     PRIMARY KEY(id),
--     FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
--     FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
-- );
-- CREATE TABLE passengers (
--     flight_id INTEGER,
--     passport_number INTEGER,
--     seat TEXT,
--     FOREIGN KEY(flight_id) REFERENCES flights(id)
-- );
-- CREATE TABLE phone_calls (
--     id INTEGER,
--     caller TEXT,
--     receiver TEXT,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     duration INTEGER,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE people (
--     id INTEGER,
--     name TEXT,
--     phone_number TEXT,
--     passport_number INTEGER,
--     license_plate TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE bakery_security_logs (
--     id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     hour INTEGER,
--     minute INTEGER,
--     activity TEXT,
--     license_plate TEXT,
--     PRIMARY KEY(id)
-- );


--find the crime scene report:
SELECT * FROM crime_scene_reports WHERE description LIKE "%duck%";
--
--------+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
--| id  | year | month | day |     street      |                                                                                                       description                                                                                                        |
--------+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
--- 295 | 2021 | 7     | 28  | Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
--------+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- good query will be 'WHERE year = 2021 AND month = 7 AND day = 28;


-- other query can be TIME ~ 10:15 AM, Street = Humphrey Street, Store = Bakery

--Query to check data in interviews after or on  day of the theft:
SELECT * FROM interviews WHERE day = 28 AND month = 7 AND year >= 2021 AND transcript LIKE '%bakery%';
-- +-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- | id  |  name   | year | month | day |                                                                                                                                                     transcript                                                                                                                                                      |
-- +-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- | 161 | Ruth    | 2021 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
-- | 162 | Eugene  | 2021 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- | 163 | Raymond | 2021 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
-- +-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

-- first interview says to check bakery cameras for cars:
SELECT COUNT(DISTINCT(license_plate)) FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day =28;
-- There are 37 License plates to check
--cross refrencing with license plate with the people databate, we have our list of initial suspects. within 10 minutes of theft.
SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute <= 25) ORDER BY name;
-- +---------+
-- |  name   |
-- +---------+
-- | Barry   |
-- | Brandon |
-- | Bruce   |
-- | Diana   |
-- | Iman    |
-- | Kelsey  |
-- | Luca    |
-- | Sofia   |
-- | Sophia  |
-- | Vanessa |
-- +---------+

--Second note says they used the atm on legget street that same day to withdraw some money:
SELECT * FROM atm_transactions WHERE year = 2021 AND day = 28 AND MONTH = 7 AND atm_location LIKE 'leggett street' AND transaction_type = 'withdraw';
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | 246 | 28500762       | 2021 | 7     | 28  | Leggett Street | withdraw         | 48     |
-- | 264 | 28296815       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 266 | 76054385       | 2021 | 7     | 28  | Leggett Street | withdraw         | 60     |
-- | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     |
-- | 269 | 16153065       | 2021 | 7     | 28  | Leggett Street | withdraw         | 80     |
-- | 288 | 25506511       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 313 | 81061156       | 2021 | 7     | 28  | Leggett Street | withdraw         | 30     |
-- | 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+


--cross reference these with the account number as welll as the bank account table and the people table to get another list of names.
SELECT name FROM people where id IN (SELECT person_id FROM bank_accounts WHERE account_number IN(SELECT account_number FROM atm_transactions WHERE year = 2021 AND day = 28 AND MONTH = 7 AND atm_location LIKE 'leggett street' AND transaction_type = 'withdraw'));
-- +---------+
-- |  name   |
-- +---------+
-- | Kenny   |
-- | Iman    |
-- | Benista |
-- | Taylor  |
-- | Brooke  |
-- | Luca    |
-- | Diana   |
-- | Bruce   |
-- +---------+

--The third says a call that lastes less than an munite was made.
-- cross refrencing  list of calls and people database we get a third list:
SELECT name FROM people WHERE phone_number IN (SELECT caller  FROM phone_calls WHERE year = 2021 AND month = 7 AND day =28 AND duration < 60);
-- +---------+
-- |  name   |
-- +---------+
-- | Kenny   |
-- | Sofia   |
-- | Benista |
-- | Taylor  |
-- | Diana   |
-- | Kelsey  |
-- | Kathryn |
-- | Bruce   |
-- | Carina  |
-- +---------+

--Combining all thee querys gives us:
SELECT name FROM people WHERE phone_number IN (SELECT caller  FROM phone_calls WHERE year = 2021 AND month = 7 AND day =28 AND duration <= 60) AND name IN (SELECT name FROM people where id IN (SELECT person_id FROM bank_accounts WHERE account_number IN(SELECT account_number FROM atm_transactions WHERE year = 2021 AND day = 28 AND MONTH = 7 AND atm_location LIKE 'leggett street' AND transaction_type = 'withdraw'))AND name IN (SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute <=25) ));
-- +-------+
-- | name  |
-- +-------+
-- | Diana |
-- | Bruce |
-- +-------+
--so we have three suspects for the thief.
--from the 3rd interview we also know that the acomplice bought a ticket for them for the earliest flight out. we can refrence the tables of passport numbers, to see if any of these people taveled out if the country the 29th and on the earliest flight.
SELECT * FROM flights where year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = (SELECT id FROM airports WHERE city Like 'fiftyville') ORDER BY hour, minute LIMIT 1;
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+

--so who was on that flight from our list?
 SELECT name FROM people WHERE passport_number IN (SELECT passport_number FROM passengers WHERE flight_id IN (SELECT id FROM flights where year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = (SELECT id FROM airports WHERE city Like 'fiftyville') ORDER BY hour, minute LIMIT 1));
-- +--------+
-- |  name  |
-- +--------+
-- | Kenny  |
-- | Sofia  |
-- | Taylor |
-- | Luca   |
-- | Kelsey |
-- | Edward |
-- | Bruce  |
-- | Doris  |
-- +--------+

SELECT name FROM people WHERE passport_number IN (SELECT passport_number FROM passengers WHERE flight_id IN (SELECT id FROM flights where year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = (SELECT id FROM airports WHERE city Like 'fiftyville') ORDER BY hour, minute LIMIT 1)) INTERSECT SELECT name FROM people WHERE phone_number IN (SELECT caller  FROM phone_calls WHERE year = 2021 AND month = 7 AND day =28 AND duration <= 60) AND name IN (SELECT name FROM people where id IN (SELECT person_id FROM bank_accounts WHERE account_number IN(SELECT account_number FROM atm_transactions WHERE year = 2021 AND day = 28 AND MONTH = 7 AND atm_location LIKE 'leggett street' AND transaction_type = 'withdraw'))AND name IN (SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute <= 25) ));
-- +-------+
-- | name  |
-- +-------+
-- | Bruce |
-- +-------+

--so we have our thief. now we find out who he talked to and where he went.
--we know the flight:
SELECT city FROM airports WHERE id =(SELECT destination_airport_id FROM flights where year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = (SELECT id FROM airports WHERE city Like 'fiftyville') ORDER BY hour, minute LIMIT 1);
-- +---------------+
-- |     city      |
-- +---------------+
-- | New York City |
-- +---------------+
--and now the accomplice:
SELECT name FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE caller IN (SELECT phone_number FROM people WHERE name LIKE 'bruce') AND year = 2021 AND month = 7 AND day =28 AND duration < 60 );
-- +-------+
-- | name  |
-- +-------+
-- | Robin |
-- +-------+