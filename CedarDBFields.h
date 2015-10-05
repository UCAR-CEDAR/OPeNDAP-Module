// CedarDBFields.h

#ifndef I_CedarDBFields_h
#define I_CedarDBFields_h 1

#include <string>
#include <sstream>

using std::string ;
using std::ostringstream ;

class CedarDBColumn
{
private:
    string		_name ;
    string		_svalue ;
    bool		_is_svalue ;
    int			_ivalue ;
    bool		_is_ivalue ;
public:
			CedarDBColumn( const string &in_name,
				       const string &in_value )
			: _name( in_name ),
			  _svalue( in_value ), _is_svalue( true ),
			  _ivalue( 0 ), _is_ivalue( false ) {}
			CedarDBColumn( const string &in_name,
				       int in_value )
			: _name( in_name ),
			  _svalue( "" ), _is_svalue( false ),
			  _ivalue( in_value ), _is_ivalue( true ) {}
    string		name() const { return _name ; }
    string		update() const
			{
			    ostringstream result ;
			    result << _name << " = " ;
			    if( _is_svalue ) result << "'" << _svalue << "'" ;
			    else result << _ivalue ;
			    return result.str() ;
			}
    string		insert() const
			{
			    ostringstream result ;
			    if( _is_svalue ) result << "'" << _svalue << "'" ;
			    else result << _ivalue ;
			    return result.str() ;
			}
} ;

class CedarDBWhere
{
private:
    string		_name ;
    string		_comp ;
    string		_svalue ;
    bool		_is_svalue ;
    int			_ivalue ;
    bool		_is_ivalue ;
    string		_op ;
    bool		_has_op ;
public:
			CedarDBWhere( const string &in_name,
				      const string &in_comp,
				      const string &in_value )
			: _name( in_name ), _comp( in_comp ),
			  _svalue( in_value ), _is_svalue( true ),
			  _ivalue( 0 ), _is_ivalue( false ),
			  _op( "" ), _has_op( false ) {}
			CedarDBWhere( const string &in_name,
				      const string &in_comp,
				      int in_value )
			: _name( in_name ), _comp( in_comp ),
			  _svalue( "" ), _is_svalue( false ),
			  _ivalue( in_value ), _is_ivalue( true ),
			  _op( "" ), _has_op( false ) {}
			CedarDBWhere( const string &in_op,
				      const string &in_name,
				      const string &in_comp,
				      const string &in_value )
			: _name( in_name ), _comp( in_comp ),
			  _svalue( in_value ), _is_svalue( true ),
			  _ivalue( 0 ), _is_ivalue( false ),
			  _op( in_op ), _has_op( true ) {}
			CedarDBWhere( const string &in_op,
				      const string &in_name,
				      const string &in_comp,
				      int in_value )
			: _name( in_name ), _comp( in_comp ),
			  _svalue( "" ), _is_svalue( false ),
			  _ivalue( in_value ), _is_ivalue( true ),
			  _op( in_op ), _has_op( true ) {}
    string		where() const
			{
			    ostringstream result ;
			    if( _has_op ) result << _op << " " ;
			    result << _name << " " << _comp << " " ;
			    if( _is_svalue ) result << "'" << _svalue << "'" ;
			    else result << _ivalue ;
			    return result.str() ;
			}
} ;

#endif // I_CedarDBFields_h

