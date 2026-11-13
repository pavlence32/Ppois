#### Lab 2 build and tests

```bash
cd lab2
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Запуск программы:

```bash
cmake --build build --target lab2_cli
./build/lab2_cli
```

### Lab 2 class model

The table counts declared fields and public methods in the corresponding
headers. Constructors and destructors are included in the method count.

| Class | Fields | Methods | Associations (related classes) |
|---|---:|---:|---|
| Money | 1 | 10 | — |
| Address | 3 | 0 | — |
| Exception | 1 | 2 | Specialized exceptions |
| People | 4 | 7 | — |
| employee | 5 | 2 | People |
| Guest | 9 | 9 | People |
| Chef | 3 | 3 | employee |
| Housekeeper | 3 | 3 | employee |
| receptionist | 3 | 3 | employee |
| HotelManager | 5 | 6 | Chef, Housekeeper, receptionist |
| GuestBadge | 2 | 4 | — |
| GuestFeedback | 3 | 4 | — |
| Review | 3 | 4 | — |
| Complaint | 3 | 4 | — |
| Bed | 4 | 4 | — |
| RoomType | 5 | 3 | — |
| Amenity | 8 | 9 | Money |
| Room | 26 | 25 | Floor, Amenity, Date, Money |
| StandardRoom | 4 | 5 | Room |
| DeluxeRoom | 5 | 6 | Room, ViewType |
| Suite | 8 | 7 | Room, Service |
| Floor | 5 | 10 | Hotel, Room |
| RoomKey | 8 | 8 | Room, Guest, Date |
| KeyCard | 3 | 4 | Date |
| RoomInspection | 3 | 4 | — |
| Service | 6 | 6 | Money |
| BreakfastOrder | 3 | 4 | Money |
| LaundryOrder | 3 | 4 | Money |
| MaintenanceTask | 3 | 4 | — |
| MiniBar | 3 | 4 | Money |
| RoomServiceOrder | 2 | 4 | Money |
| SpaBooking | 3 | 4 | Money |
| TransportRequest | 3 | 4 | Money |
| PoolVisit | 3 | 4 | Date |
| GuestRequest | 16 | 13 | Guest, Room, Money, Date |
| Reservation | 26 | 25 | Guest, Room, BookingPolicy, AvailabilityCalendar, Date, Money |
| CancellationPolicy | 6 | 8 | Reservation, BookingPolicy, Date, Money |
| AvailabilityCalendar | 4 | 9 | Room, Reservation, Date |
| WaitList | 4 | 10 | Guest, GuestRequest, Room, Reservation, AvailabilityCalendar |
| Stay | 9 | 12 | Reservation, RoomKey, Hotel, AvailabilityCalendar, Date, Money |
| ReservationNote | 3 | 4 | Date |
| Hotel | 21 | 10 | Floor, Room, Employee, Address |
| ConferenceBooking | 3 | 4 | Money |
| HotelEvent | 3 | 4 | Date |
| HouseRule | 3 | 4 | — |
| ParkingTicket | 3 | 4 | — |
| Discount | 3 | 4 | Money, Date |
| Invoice | 3 | 5 | Money |
| Payment | 3 | 5 | Money |

#### Lab 2 exceptions

- EmployeeUnavailableException
- InvalidDatesException
- RoomNotAvailableException
- RoomNotCleanedException
- RoomNotFoundException
- RoomOverCapacityException
- DuplicateRoomException
- InvalidKeyException
- KeyExpiredException
- InvalidDateException
- InvalidPaymentException
- InsufficientFundsException
- OverbookingException
- InvalidReservationStateException
- CancellationNotAllowedException
- ReservationNotFoundException
- WaitListFullException

#### Lab 2 model statistics

| Metric | Value |
|---|---:|
| Declared types | 49 |
| Fields | 266 |
| Methods | 305 |
| Associations | 69 |
| Specialized exceptions | 17 |