#ifndef _KALI_IPROJECTOR_HPP_
#define	_KALI_IPROJECTOR_HPP_

namespace kaliscope
{

class IProjector {
public:
    /**
     * @brief constructor
     * @param maxActivatedTime[in] maximum time in which the projector in switched on, -1 = unlimited
     */
    IProjector( const int maxActivatedTime = -1 )
    : _maxActiveTime( maxActivatedTime )
    {}

    virtual ~IProjector() = 0;
    
    /**
     * @brief initialize projector
     */
    virtual void init() = 0;

    /**
     * @brief switch the projector on or off
     * @param on[in] activate or disable projector
     */
    void switchProjector( const bool on = true )
    {
        if ( on )
        {
            switchOn();
        }
        else
        {
            switchOff();
        }
    }

    /**
     * @brief switch projector on
     */
    virtual void switchOn() = 0;

    /**
     * @brief switch projector off
     */
    virtual void switchOff() = 0;

protected:
    bool _active = false;
    double _maxActiveTime = -1;
};

}

#endif
