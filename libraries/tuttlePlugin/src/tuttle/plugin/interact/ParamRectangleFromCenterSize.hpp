#ifndef _TUTTLE_PLUGIN_INTERACT_PARAMRECTANGLEFROMCENTERSIZE_HPP_
#define _TUTTLE_PLUGIN_INTERACT_PARAMRECTANGLEFROMCENTERSIZE_HPP_

#include "InteractObject.hpp"
#include "PointInteract.hpp"
#include "ParamPoint.hpp"
#include "overlay.hpp"
#include <terry/globals.hpp>
#include <tuttle/plugin/ofxToGil/point.hpp>

#include <ofxsParam.h>

#include <boost/lexical_cast.hpp>

namespace tuttle {
namespace plugin {
namespace interact {

template<ECoordinateSystem coord>
struct CoordinateSystemNotCentered
{
	static const ECoordinateSystem value = coord;
};
template<>
struct CoordinateSystemNotCentered<eCoordinateSystemXXcn>
{
	static const ECoordinateSystem value = eCoordinateSystemXXn;
};
template<>
struct CoordinateSystemNotCentered<eCoordinateSystemXYc>
{
	static const ECoordinateSystem value = eCoordinateSystemXYc;
};

template<class TFrame, ECoordinateSystem coord>
class ParamRectangleFromCenterSize : public PointInteract
{
public:
	ParamRectangleFromCenterSize( const InteractInfos& infos, OFX::Double2DParam* paramCenter, OFX::Double2DParam* paramSize, const TFrame& frame );
	~ParamRectangleFromCenterSize();

private:
	ParamPoint<TFrame, coord> _center;
	ParamPoint<TFrame, CoordinateSystemNotCentered<coord>::value> _size;
	TFrame _frame;

	/**
	 *         T
	 *  TL___________ TR
	 *    |         |
	 * L  |    C    |  R
	 *    |         |
	 *    |_________|
	 *  BL           BR
	 *         B
	 */
	enum ESelectType
	{
		eSelectTypeNone,
		eSelectTypeT,
		eSelectTypeL,
		eSelectTypeR,
		eSelectTypeB,
		eSelectTypeTL,
		eSelectTypeTR,
		eSelectTypeBL,
		eSelectTypeBR,
		eSelectTypeC
	};
	static std::string mapESelectTypeToString( const ESelectType& s )
	{
		switch( s )
		{
			case eSelectTypeNone:
				return "eSelectTypeNone";
			case eSelectTypeT:
				return "eSelectTypeT";
			case eSelectTypeL:
				return "eSelectTypeL";
			case eSelectTypeR:
				return "eSelectTypeR";
			case eSelectTypeB:
				return "eSelectTypeB";
			case eSelectTypeTL:
				return "eSelectTypeTL";
			case eSelectTypeTR:
				return "eSelectTypeTR";
			case eSelectTypeBL:
				return "eSelectTypeBL";
			case eSelectTypeBR:
				return "eSelectTypeBR";
			case eSelectTypeC:
				return "eSelectTypeC";
		}
		BOOST_THROW_EXCEPTION( std::invalid_argument( "ESelectType: " + boost::lexical_cast<std::string>( s ) ) );
		return "";
	}

	ESelectType _selectType;

public:
	bool draw( const OFX::DrawArgs& args ) const;

	ESelectType selectType( const OFX::PenArgs& args ) const;

	MotionType intersect( const OFX::PenArgs& args );
	bool      isIn( const OfxRectD& );

	Point2 getPoint() const
	{
		return _center.getPoint();
	}

	void setPoint( const Scalar x, const Scalar y )
	{
		if( _selectType == eSelectTypeC )
			return _center.setPoint( x, y );

		Point2 pCenter = _center.getPoint();
		Point2 pSize   = _size.getPoint();

		OfxRectD frameRect = _frame.getFrame( this->getTime() );
		Point2 mouse( x - frameRect.x1, y - frameRect.y1 ); // relative to frame
		mouse = mouse - pCenter; // relative to pointCenter
		switch( _selectType )
		{
			case eSelectTypeL:
			{
				_size.setPoint( std::abs( mouse.x ), pSize.y );
				break;
			}
			case eSelectTypeR:
			{
				_size.setPoint( std::abs( mouse.x ), pSize.y );
				break;
			}
			case eSelectTypeT:
			{
				_size.setPoint( pSize.x, std::abs( mouse.y ) );
				break;
			}
			case eSelectTypeB:
			{
				_size.setPoint( pSize.x, std::abs( mouse.y ) );
				break;
			}
			case eSelectTypeTL:
			{
				// todo
				break;
			}
			case eSelectTypeTR:
			{
				// todo
				break;
			}
			case eSelectTypeBL:
			{
				// todo
				break;
			}
			case eSelectTypeBR:
			{
				// todo
				break;
			}
			case eSelectTypeC:
			{
				assert( false );
				break;
			}
			case eSelectTypeNone:
			{
				assert( false );
				break;
			}
		}
	}

};

template<class TFrame, ECoordinateSystem coord>
ParamRectangleFromCenterSize<TFrame, coord>::ParamRectangleFromCenterSize( const InteractInfos& infos, OFX::Double2DParam* paramCenter, OFX::Double2DParam* paramSize, const TFrame& frame )
	: PointInteract( infos )
	, _center( infos, paramCenter, frame )
	, _size( infos, paramSize, frame )
	, _frame( frame )
{}

template<class TFrame, ECoordinateSystem coord>
ParamRectangleFromCenterSize<TFrame, coord>::~ParamRectangleFromCenterSize() {}

template<class TFrame, ECoordinateSystem coord>
bool ParamRectangleFromCenterSize<TFrame, coord>::draw( const OFX::DrawArgs& args ) const
{
	_center.draw( args );
	overlay::displayPointRect( _center.getPoint(), _size.getPoint() );
	return true;
}

template<class TFrame, ECoordinateSystem coord>
typename ParamRectangleFromCenterSize<TFrame, coord>::ESelectType ParamRectangleFromCenterSize<TFrame, coord>::selectType( const OFX::PenArgs& args ) const
{
	const Point2 p              = ofxToGil( args.penPosition );
	const double margeCanonical = this->getMarge() * args.pixelScale.x;

	TUTTLE_LOG_VAR( TUTTLE_TRACE, this->getMarge() );
	TUTTLE_LOG_VAR( TUTTLE_TRACE, args.pixelScale.x );
	TUTTLE_LOG_VAR( TUTTLE_TRACE, margeCanonical );
	const OfxRectD rod = _frame.getFrame( this->getTime() );
	//const Point2 rodSize( rod.x2 - rod.x1, rod.y2 - rod.y1 );
	const Point2 pCenter = _center.getPoint();
	const Point2 pSize   = _size.getPoint();
	const Point2 min     = pCenter - pSize;
	const Point2 max     = pCenter + pSize;

	if( std::abs( min.x - p.x ) < margeCanonical ) // left
	{
		if( std::abs( min.y - p.y ) < margeCanonical ) // top
			return eSelectTypeTL;
		else if( std::abs( max.y - p.y ) < margeCanonical ) // bottom
			return eSelectTypeBL;
		else if( p.y > min.y && p.y < max.y )
			return eSelectTypeL;
	}
	else if( std::abs( max.x - p.x ) < margeCanonical ) // right
	{
		if( std::abs( max.y - p.y ) < margeCanonical ) // bottom
			return eSelectTypeBR;
		else if( std::abs( min.y - p.y ) < margeCanonical ) // top
			return eSelectTypeTR;
		else if( p.y > min.y && p.y < max.y )
			return eSelectTypeR;
	}
	else if( std::abs( min.y - p.y ) < margeCanonical && p.x > min.x && p.x < max.x )
		return eSelectTypeT;
	else if( std::abs( max.y - p.y ) < margeCanonical && p.x > min.x && p.x < max.x )
		return eSelectTypeB;
	return eSelectTypeNone;
}

template<class TFrame, ECoordinateSystem coord>
MotionType ParamRectangleFromCenterSize<TFrame, coord>::intersect( const OFX::PenArgs& args )
{
	// intersect center point
	MotionType m = _center.intersect( args );
	if( m._mode != eMotionNone )
	{
		TUTTLE_TLOG( TUTTLE_TRACE, "intersect center." );
		_selectType = eSelectTypeC;
		return m;
	}
	// intersect borders
	_selectType = selectType( args );
	TUTTLE_TLOG( TUTTLE_TRACE, "_selectType : " << mapESelectTypeToString( _selectType ) );
	if( _selectType != eSelectTypeNone )
	{
		m._mode = eMotionTranslate;
		m._axis = eAxisXY;
		return m;
	}
	m._mode = eMotionNone;
	m._axis = eAxisNone;
	return m;
}

template<class TFrame, ECoordinateSystem coord>
bool ParamRectangleFromCenterSize<TFrame, coord>::isIn( const OfxRectD& rect )
{
	_selectType = eSelectTypeNone;
	Point2 pCenter = _center.getPoint();
	Point2 pSize   = _size.getPoint();
	Point2 min     = pCenter - pSize;
	Point2 max     = pCenter + pSize;
	if( rect.x1 <= min.x  && rect.x2 >= max.x &&
	    rect.y1 <= min.y  && rect.y2 >= max.y )
	{
		return true;
	}
	return false;
}

}
}
}

#endif

