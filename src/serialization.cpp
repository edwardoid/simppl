#include "simppl/serialization.h"
#include <sstream>

simppl::dbus::DecoderError::DecoderError(int expected, int actual)
{
   std::stringstream ss;
   ss << "Expected: " << expected << '(' << (char)(expected) << ") Got: " << actual << '(' << (char)(actual) << ')';
   m_msg = ss.str();
}

const char* simppl::dbus::DecoderError::what() const noexcept
{
   return m_msg.c_str();
}

void simppl_dbus_message_iter_recurse(DBusMessageIter* iter, DBusMessageIter* nested, int expected_type)
{
   const int actual = dbus_message_iter_get_arg_type(iter); 
   if (actual != expected_type)
      throw simppl::dbus::DecoderError(expected_type, actual);
      
   dbus_message_iter_recurse(iter, nested);
}


void simppl_dbus_message_iter_get_basic(DBusMessageIter* iter, void* p, int expected_type)
{
   const int actual = dbus_message_iter_get_arg_type(iter); 
   if (actual != expected_type)
      throw simppl::dbus::DecoderError(expected_type, actual);
   
   dbus_message_iter_get_basic(iter, p);
   dbus_message_iter_next(iter);
}
