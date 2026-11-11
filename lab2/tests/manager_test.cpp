#include <catch2/catch_test_macros.hpp>
#include "people/manager.hpp"
TEST_CASE("Manager assigns employees") { HotelManager m; Chef c(30,"C","M",1,"cook"); Housekeeper h(30,"H","F",2,"clean"); receptionist r(30,"R","F",3,"front"); r.set_status(false); m.addChef(c); m.addHousekeeper(h); m.addReceptionist(r); m.assignChef(); m.assignHousekeeper(101); m.assignReceptionist(); }
