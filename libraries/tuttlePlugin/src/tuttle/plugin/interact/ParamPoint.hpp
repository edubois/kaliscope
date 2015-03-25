#ifndef _TUTTLE_PLUGIN_PARAMPOINT_HPP_
#define _TUTTLE_PLUGIN_PARAMPOINT_HPP_

#include "Frame.hpp"
#include "InteractInfos.hpp"
#include "InteractObject.hpp"
#include "PointInteract.hpp"
#include "interact.hpp"
#include <tuttle/plugin/ofxToGil/point.hpp>
#include <tuttle/plugin/numeric/coordinateSystem.hpp>

#include <ofxsParam.h>

namespace tuttle {
namespace plugin {
namespace interact {

template<class TFrame, ECoordinateSystem coord>
class ParamPoint : public PointInteract
{
public:
	ParamPoint( const InteractInfos& infos, OFX::Double2DParam* param, const TFrame& frame )
		: PointInteract( infos )
		, _param( *param )
		, _frame( frame )
	{}

	virtual ~ParamPoint() {}

protected:
	OFX::Double2DParam& _param;
	const TFrame _frame;

public:
	Point2 getPoint() const
	{
		if( _frame.isEnabled() )
		{
			OfxRectD rod = _frame.getFrame( this->getTime() );
			Point2 rodSize( rod.x2 - rod.x1, rod.y2 - rod.y1 );
			Point2 p = pointConvertCoordinateSystem<coord, eCoordinateSystemXY>( ofxToGil( _param.getValue() ), rodSize );
			p += Point2( rod.x1, rod.y1 );
			return p;
		}
		return Point2( 0, 0 );
	}

	void setPoint( const Scalar x, const Scalar y )
	{
		if( _frame.isEnabled() )
		{
			OfxRectD rod = _frame.getFrame( this->getTime() );
			Point2 rodSize( rod.x2 - rod.x1, rod.y2 - rod.y1 );
			Point2 p( x - rod.x1, y - rod.y1 );
			p = pointConvertCoordinateSystem<eCoordinateSystemXY, coord>( p, rodSize );
			_param.setValue( p.x, p.y );
			return;
		}
		_param.setValue( 0, 0 );
	}

};

}
}
}

#endif

