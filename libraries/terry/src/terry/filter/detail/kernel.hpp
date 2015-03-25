/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef _TERRY_NUMERIC_KERNEL_HPP_
#define _TERRY_NUMERIC_KERNEL_HPP_

/*!
/// \file
/// \brief Definitions of 1D fixed-size and variable-size kernels and related operations
/// \author Hailin Jin and Lubomir Bourdev \n
///         Adobe Systems Incorporated
/// \date   2005-2007 \n Last updated on September 26, 2006
*/


#include <boost/gil/gil_config.hpp>
#include <boost/gil/utilities.hpp>

#include <boost/array.hpp>
#include <boost/mpl/bool.hpp>

#include <cstddef>
#include <cassert>
#include <algorithm>
#include <vector>
#include <memory>

namespace terry {
namespace filter {

using namespace boost::gil;

namespace detail {

/// \brief kernel adaptor for one-dimensional cores
/// Core needs to provide size(),begin(),end(),operator[],
///                       value_type,iterator,const_iterator,reference,const_reference
template <typename Core>
class kernel_1d_adaptor : public Core {
private:
    std::size_t _center;
public:
    kernel_1d_adaptor() : _center(0) {}
    explicit kernel_1d_adaptor(std::size_t center_in) : _center(center_in) {assert(_center<this->size());}
    kernel_1d_adaptor(std::size_t size_in,std::size_t center_in) :
        Core(size_in), _center(center_in) {assert(_center<this->size());}
    kernel_1d_adaptor(const kernel_1d_adaptor& k_in) : Core(k_in), _center(k_in._center) {}

    kernel_1d_adaptor& operator=(const kernel_1d_adaptor& k_in) {
        Core::operator=(k_in);
        _center=k_in._center;
        return *this;
    }
    std::size_t left_size() const {assert((_center<this->size()) || (this->size()==0));return _center;}
    std::size_t right_size() const {assert((_center<this->size()) || (this->size()==0));return this->size()?this->size()-_center-1:0;}
          std::size_t& center()       {return _center;}
    const std::size_t& center() const {return _center;}
    void set_center( const std::size_t center ) { _center = center; }
};

} // namespace detail

/// \brief variable-size kernel
template <typename T, typename Alloc = std::allocator<T> >
class kernel_1d : public detail::kernel_1d_adaptor<std::vector<T,Alloc> > {
    typedef detail::kernel_1d_adaptor<std::vector<T,Alloc> > parent_t;
public:
	typedef T value_type;
	typedef boost::mpl::false_ is_fixed_size_t;
public:
    kernel_1d() {}
    kernel_1d(std::size_t size_in,std::size_t center_in) : parent_t(size_in,center_in) {}
    template <typename FwdIterator>
    kernel_1d(FwdIterator elements, std::size_t size_in, std::size_t center_in) : parent_t(size_in,center_in) {
        boost::gil::detail::copy_n(elements,size_in,this->begin());
    }
    kernel_1d(const kernel_1d& k_in)                     : parent_t(k_in) {}
};

/// \brief static-size kernel
template <typename T,std::size_t Size>
class kernel_1d_fixed : public detail::kernel_1d_adaptor<boost::array<T,Size> > {
    typedef detail::kernel_1d_adaptor<boost::array<T,Size> > parent_t;
public:
	typedef T value_type;
	typedef boost::mpl::true_ is_fixed_size_t;
public:
    kernel_1d_fixed() {}
    explicit kernel_1d_fixed(std::size_t center_in) : parent_t(center_in) {}
    
    template <typename FwdIterator>
    explicit kernel_1d_fixed(FwdIterator elements, std::size_t center_in) : parent_t(center_in) {
        boost::gil::detail::copy_n(elements,Size,this->begin());
    }
    kernel_1d_fixed(const kernel_1d_fixed& k_in)    : parent_t(k_in) {}
};

/// \brief reverse a kernel
template <typename Kernel>
inline Kernel reverse_kernel(const Kernel& kernel) {
    Kernel result(kernel);
    result.center()=kernel.right_size();
    std::reverse(result.begin(), result.end());
    return result;
}


}
}

#endif
