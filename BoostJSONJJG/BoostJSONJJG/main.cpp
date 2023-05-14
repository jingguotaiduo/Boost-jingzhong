//#include <iostream>
//#include <iterator>
//#include <algorithm>
//#include <boost/lambda/lambda.hpp>
//
//int main()
//{
//	using namespace boost::lambda;
//	typedef std::istream_iterator<int> in;
//
//	std::for_each(
//		in(std::cin), in(), std::cout << (_1 * 3) << " ");
//}

//#include <string>
//#include <set>
//#include <exception>
//#include <iostream>
//
//#include <boost/foreach.hpp>
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/xml_parser.hpp>
//
//struct debug_settings
//{
//	std::string m_file;
//	int m_level;
//	std::set<std::string> m_modules;
//	void load(const std::string &filename);
//	void save(const std::string &filename);
//};
//
//void debug_settings::load(const std::string &filename)
//{
//	using boost::property_tree::ptree;
//	ptree pt;
//	read_xml(filename, pt);
//	m_file = pt.get<std::string>("debug.filename");
//	m_level = pt.get("debug.level", 0);
//	BOOST_FOREACH(ptree::value_type &v, pt.get_child("debug.modules"))
//		m_modules.insert(v.second.data());
//}
//
//void debug_settings::save(const std::string &filename)
//{
//	using boost::property_tree::ptree;
//	ptree pt;
//	pt.put("debug.filename", m_file);
//	pt.put("debug.level", m_level);
//	BOOST_FOREACH(const std::string &name, m_modules)
//		pt.add("debug.modules.module", name);
//	write_xml(filename, pt);
//}
//
//int main()
//{
//	try
//	{
//		debug_settings ds;
//		ds.load("debug_settings.xml");
//		ds.save("debug_settings_out.xml");
//		std::cout << "Success\n";
//	}
//	catch (std::exception &e)
//	{
//		std::cout << "Error: " << e.what() << "\n";
//	}
//
//	system("pause");
//	return 0;
//}

#include <iostream>
#include <boost/json.hpp>
#include <boost/json/parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using namespace boost::json;
using namespace boost::property_tree;

void json_function_test()
{
	boost::json::value value = boost::json::parse("{ \"test\": true }");// 文档模型中的容器类型分为数组（array)、对象（object）、字符串（string)、值（value）
	std::cout << value << std::endl;
	boost::json::value jv(boost::json::object_kind);
	assert(jv.kind() == boost::json::kind::object);// 判断值的类型
	assert(jv.is_object());// 判断值是否为object类型
	assert(!jv.is_number());// 判断值是否为number类型
	assert(jv.is_array());// 判断值是否为array类型
	assert(jv.is_string());// 判断值是否为string类型
	object jo1 = { { "john", 100 },{ "dave", 500 },{ "joe", 300 } };

	boost::json::object obj;
	obj["pi"] = 3.141;
	obj["happy"] = true;
	obj["name"] = "Boost";
	obj["nothing"] = nullptr;
	obj["answer"].emplace_object()["everything"] = 42;
	obj["list"] = { 1, 0, 2 };
	obj["object"] = { { "currency", "USD" },{ "value", 42.99 } };
	std::cout << obj << std::endl;
	std::cout << obj.at("name") << std::endl;

	object obj2({ { "key1", "value1" },{ "key2", 42 },{ "key3", false } });
	boost::json::value jv_array = { 1, 2, 3 };
	assert(jv_array.is_array());
	std::string s = boost::json::serialize(jv_array); //值序列化为字符串
	std::cout << s << std::endl;

	boost::json::value jv1 = { { "hello", 42 },{ "world", 43 } };
	assert(jv1.is_object());
	assert(jv1.as_object().size() == 2);
	assert(serialize(jv1) == R"({"hello":42,"world":43})");
}

void write_json(std::string out_jsonFile)
{
	using namespace boost::property_tree;
	ptree root;
	ptree arr;
	root.put("version", 1);
	ptree node1, node2;
	node1.put("frame_rate", 0);
	node1.put("bit_rate", 1);
	node1.put("resolution", 3);
	node1.put("boot", 0);
	node1.put("ssfs", 2);
	root.add_child("first", node1);
	root.add_child("second", node1);
	root.add_child("third", node1);

	stringstream s;
	write_json(s, root, false);
	std::string out = s.str();
	ofstream oftream(out_jsonFile, std::ios_base::out);
	oftream << out;
	oftream.close();
}

void read_json(std::string in_jsonFile)
{
	using namespace boost::property_tree;
	ifstream ifs(in_jsonFile, std::ios_base::in);
	ptree node;//根节点
	read_json(ifs, node); // 将json文件读入根节点
	cout << "根节点包含" << node.size() << "个孩子节点！" << endl;
	for (boost::property_tree::ptree::iterator it = node.begin(); it != node.end(); ++it)
	{
		if (it->first != "version")
		{
			cout << it->first << " : " << endl;
			ptree node2 = node.get_child(it->first);// it->second;
			for (boost::property_tree::ptree::iterator it2 = node2.begin(); it2 != node2.end(); ++it2)
			{
				cout << it2->first << " : " << it2->second.data() << endl;
			}
		}
		else
		{
			cout << it->first << " : " << it->second.data() << endl; // key : value
		}
	}
	ifs.close();
}

int main()
{
	cout << "开始测试json函数：" << endl;
	json_function_test();
	cout << "json函数测试完毕！：" << endl;

	cout << "开始写入json文件（config_out.json）：" << endl;
	write_json("config_out.json");
	cout << "json文件写入完毕！" << endl;

	cout << "开始读取json文件（config_out.json）：" << endl;
	read_json("config_out.json");
	cout << "json文件读取完毕！" << endl;
	system("pause");
	return 0;
}