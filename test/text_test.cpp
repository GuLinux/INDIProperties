#include "gtest/gtest.h"
#include "indi_property.h"
#include "indi_functions_impl.h"
#include "text.h"
#include "c++/containers_streams.h"
using namespace INDI::Properties;
using namespace std;
using namespace GuLinux;
TEST(INDIText, AddProperty) {
  Property<Text> my_prop{nullptr, {}, [](const vector<Text::UpdateArgs> &){ return false; }};
  my_prop.add("prop name", "prop label", "hello");
  my_prop.add("prop name2", "prop label2", "world");
  ASSERT_EQ(2, my_prop.properties().size());
  ASSERT_STREQ("prop label", my_prop.properties()[0].label);
  ASSERT_STREQ("prop label2", my_prop.properties()[1].label);
  ASSERT_STREQ("hello", my_prop.properties()[0].text);
  ASSERT_STREQ("world", my_prop.properties()[1].text);
  ASSERT_EQ(2, my_prop.vector_property().ntp);
}


TEST(INDIText, UpdateProperty) {
  bool retval = true;
  vector<Text::vtype> values(2);
  auto update_f = [&](const vector<Text::UpdateArgs> & new_values){
    for(int i=0; i<new_values.size(); i++)
      values[i] = get<0>(new_values[i]);
    return retval;
  };
  const char *names[] = {"prop name", "prop name2"};
  Text::vtype new_values[] = {"hello2", "world2"};
  Property<Text> my_prop{nullptr, {"device", "name"}, update_f};
  my_prop.add("prop name", "prop label", "hello")
	 .add("prop name2", "prop label2", "world");
	 
  ASSERT_TRUE(my_prop.update("device", "name", new_values, const_cast<char**>(names), 2));
  ASSERT_EQ( (vector<Text::vtype>{new_values[0], new_values[1]}), values);
  ASSERT_EQ( (vector<string>{new_values[0], new_values[1]}), make_stream(my_prop.properties()).transform<vector<string>>([](IText v){ return string{v.text}; }).get() );
  retval = false;
  new_values[0] = "ciao";
  new_values[1] = "mondo";
  ASSERT_FALSE(my_prop.update("device", "name", new_values, const_cast<char**>(names), 2));
  ASSERT_EQ( (vector<Text::vtype>{"ciao", "mondo"}), values);
}

