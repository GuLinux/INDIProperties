#include "gtest/gtest.h"
#include "indi_property.h"
#include "indi_functions_impl.h"
#include "number.h"
#include "c++/containers_streams.h"
using namespace INDI::Properties;
using namespace std;
using namespace GuLinux;
TEST(INDINumber, AddProperty) {
  Property<Number> my_prop{nullptr, {}, [](double*, char**, int){ return false; }};
  my_prop.add("prop name", "prop label", 0, 5, 1, 3);
  my_prop.add("prop name2", "prop label2", 0, 6, 2, 2);
  ASSERT_EQ(2, my_prop.properties().size());
  ASSERT_STREQ("prop label", my_prop.properties()[0].label);
  ASSERT_STREQ("prop label2", my_prop.properties()[1].label);
  ASSERT_EQ(3, my_prop.properties()[0].value);
  ASSERT_EQ(2, my_prop.properties()[1].value);
  ASSERT_EQ(2, my_prop.vector_property().nnp);
}


TEST(INDINumber, UpdateProperty) {
  bool retval = true;
  vector<double> values(2);
  auto update_f = [&](double* new_values, char**, int n){
    for(int i=0; i<n; i++)
      values[i] = new_values[i];
    return retval;
  };
  char *names[] = {"prop name", "prop name2"};
  double new_values[] = {2, 4};
  Property<Number> my_prop{nullptr, {"device", "name"}, update_f};
  my_prop.add("prop name", "prop label", 0, 5, 1, 3)
	 .add("prop name2", "prop label2", 0, 6, 2, 2);
	 
  ASSERT_TRUE(my_prop.update("device", "name", new_values, names, 2));
  ASSERT_EQ( (vector<double>{2, 4}), values);
  ASSERT_EQ( (vector<double>{2, 4}), make_stream(my_prop.properties()).transform<vector<double>>([](INumber v){ return v.value; }).get() );
  retval = false;
  new_values[0] = 0;
  new_values[1] = 0;
  ASSERT_FALSE(my_prop.update("device", "name", new_values, names, 2));
  ASSERT_EQ( (vector<double>{0, 0}), values);
}

/*
TEST(INDINumber, RunOnFirst) {
  Property<Switch> my_prop{nullptr, {}, ISR_NOFMANY, [](ISState*, char**, int){ return false; }};
  my_prop.add("prop name", "prop label", ISS_OFF)
         .add("prop name2", "prop label2", ISS_OFF)
         .add("prop name3", "prop label3", ISS_ON)
         .add("prop name4", "prop label4", ISS_ON);
  Switch::Entry entry;
  my_prop->first_on_switch([&](const Switch::Entry &e){ entry = e; });
  ASSERT_EQ(2, entry.index);
  ASSERT_STREQ("prop name3", entry.iswitch.name);
}

TEST(INDINumber, RunOnAll) {
  Property<Switch> my_prop{nullptr, {}, ISR_NOFMANY, [](ISState*, char**, int){ return false; }};
  my_prop.add("prop name", "prop label", ISS_OFF)
          .add("prop name2", "prop label2", ISS_OFF)
          .add("prop name3", "prop label3", ISS_ON)
          .add("prop name4", "prop label4", ISS_ON);
  vector<Switch::Entry> entries;
  my_prop->on_switches([&](const Switch::Entry &e){ entries.push_back( e ); });
  ASSERT_EQ(2, entries.size());
  ASSERT_EQ(2, entries[0].index);
  ASSERT_STREQ("prop name3", entries[0].iswitch.name);
  ASSERT_EQ(3, entries[1].index);
  ASSERT_STREQ("prop name4", entries[1].iswitch.name);
}*/