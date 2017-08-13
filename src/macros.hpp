/*
 * macros.hpp
 *
 *  Created on: May 25, 2017
 *      Author: nullifiedcat
 */

#ifndef MACROS_HPP_
#define MACROS_HPP_

#if NOGUI or NO_RENDERING
#define ENABLE_GUI false
#else
#define ENABLE_GUI true
#endif

#ifndef DATA_PATH
#define DATA_PATH "/opt/cathook"
#endif

#endif /* MACROS_HPP_ */
