#include "gtest/gtest.h"
#include "indi_property.h"
#include "indi_functions_impl.h"
#include "switch.h"
using namespace INDI::Properties;
using namespace std;
TEST(INDISwitch, AddProperty) {
  Property<Switch> my_prop{nullptr, {}, ISR_1OFMANY};
  my_prop.add("prop name", "prop label", ISS_ON);
  my_prop.add("prop name2", "prop label2", ISS_OFF);
  ASSERT_EQ(2, my_prop.properties().size());
  ASSERT_STREQ("prop label", my_prop.properties()[0].label);
  ASSERT_STREQ("prop label2", my_prop.properties()[1].label);
  ASSERT_EQ(ISS_ON, my_prop.properties()[0].s);
  ASSERT_EQ(ISS_OFF, my_prop.properties()[1].s);
  ASSERT_EQ(2, my_prop.vector_property().nsp);
  ASSERT_EQ(ISR_1OFMANY, my_prop.vector_property().r);
}