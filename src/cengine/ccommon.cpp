#include "cengine.hpp"
#include <cstring>
#include <memory>

#include "3rd/colorprintf.hpp"
extern "C"
{
    int common_colorprint(lua_State* L)
    {
        luaL_checkinteger(L, 1);
        luaL_checkstring(L, 2);

        int color = static_cast<int>(lua_tointeger(L, 1));
        const char* content = lua_tostring(L, 2);

        colorprintf(color, "%s\n", content);

        return 0;
    }
}

#include "3rd/rapidxml/rapidxml.hpp"
#include "3rd/rapidxml/rapidxml_utils.hpp"
extern "C"
{
    namespace detail
    {
        void xml_make_array(lua_State* L, const char *array_name)
        {
            lua_pushstring(L, array_name);
            lua_newtable(L);
            lua_settable(L, -3);
        }

        bool xml_node_is_number(const char* value, double *d)
        {
            char* end;
            *d = strtod(value, &end);
            if (*end != '\0') return false;
            return true;
        }

        void xml_push_value(lua_State* L, const char *value)
        {
            double d;
            if (xml_node_is_number(value, &d))
            {
                lua_pushnumber(L, d);
            }
            else
            {
                lua_pushstring(L, value);
            }
        }

        bool xml_is_array_node(rapidxml::xml_node<>* node, std::string &array_name)
        {
            // <tab>
            //     <data>...</data>
            //     <data>...</data>
            // </tab>
            auto child = node->first_node();
            if (nullptr == child)
            {
                return false;
            }

            auto sibling = child->next_sibling(child->name());
            if (sibling != nullptr)
            {
                array_name = child->name();
                return true;
            }
            return false;
        }

        void xml_deal_with_node(lua_State *L, rapidxml::xml_node<>* node)
        {
            if (nullptr == node || node->type() != rapidxml::node_element)
            {
                lua_pushstring(L, "");
                lua_settable(L, -3);
                return;
            }

            std::string array_name;
            bool is_array = xml_is_array_node(node, array_name);

            for (auto child_node = node->first_node(); child_node; child_node = child_node->next_sibling())
            {
                if (child_node->type() != rapidxml::node_element)
                {
                    continue;
                }

                if (auto value_node = child_node->first_node())
                {
                    if (is_array &&
                        // 这句话可以优化掉的，因为很少有数组节点里面还包含其他别的节点 
                        strcmp(child_node->name(), array_name.c_str()) == 0)
                    {
                        // deal with array
                        lua_pushstring(L, array_name.c_str());
                        lua_gettable(L, -2);
                        luaL_checktype(L, -1, LUA_TTABLE);
                        lua_Integer n = luaL_len(L, -1);
                        if (value_node->type() == rapidxml::node_data)
                        {
                            // 最后的叶子了
                            xml_push_value(L, value_node->value());
                        }
                        else
                        {
                            lua_newtable(L);
                            // make an array
                            std::string array_name;
                            if (xml_is_array_node(child_node, array_name))
                            {
                                xml_make_array(L, array_name.c_str());
                            }

                            xml_deal_with_node(L, child_node);
                        }
                        lua_seti(L, -2, n + 1);
                        lua_pop(L, 1);
                        continue;
                    }

                    // <tab>value</tab>
                    // 最后的叶子了
                    if (value_node->type() == rapidxml::node_data)
                    {
                        lua_pushstring(L, child_node->name());
                        xml_push_value(L, value_node->value());
                        lua_settable(L, -3);
                        continue;
                    }
                }
                
                lua_pushstring(L, child_node->name());
                lua_newtable(L);
                
                // make an array
                std::string array_name;
                if (xml_is_array_node(child_node, array_name))
                {
                    xml_make_array(L, array_name.c_str());
                }

                xml_deal_with_node(L, child_node);
                lua_settable(L, -3);
            }
        }
    }

    int common_parse_xml(lua_State* L)
    {
        luaL_checkstring(L, 1);

        const char* path = lua_tostring(L, 1);

        std::unique_ptr<rapidxml::file<>> tmp_file;
        std::unique_ptr<rapidxml::xml_document<>> doc(new rapidxml::xml_document<>());

		try
		{
            tmp_file = std::unique_ptr<rapidxml::file<>>(new rapidxml::file<>(path));
			doc->parse<0>(tmp_file->data()); // 0 means default parse flags
		}
		catch (rapidxml::parse_error e)
		{
            lua_pushnil(L);
            lua_pushstring(L, e.what());
			return 2;
		}
		catch (std::runtime_error e)
		{
            lua_pushnil(L);
            lua_pushstring(L, e.what());
            return 2;
		}

        auto root = doc->first_node();
        if (nullptr == root || root->type() != rapidxml::node_element)
        {
            lua_pushnil(L);
            lua_pushstring(L, "no root element");
            return 2;
        }

        lua_newtable(L);
        
        std::string array_name;
        if (detail::xml_is_array_node(root, array_name))
        {
            // make an array
            detail::xml_make_array(L, array_name.c_str());
        }

        detail::xml_deal_with_node(L, root);

        return 1;
    }
}

static const struct luaL_Reg common_f[] = {
    { "ColorPrintf", common_colorprint },
    { "ParseXml", common_parse_xml },
    { NULL, NULL },
};

int luaopen_common(lua_State *L)
{
    luaL_newlib(L, common_f);
    return 1;
}
