/*
 * macros.hpp
 *
 *  Created on: May 25, 2017
 *      Author: nullifiedcat
 */

#ifndef MACROS_HPP_
#define MACROS_HPP_

<<<<<<< HEAD
#if NOGUI or NO_RENDERING
=======
#if defined(NOGUI) and NOGUI == 1 or defined(TEXTMODE)
>>>>>>> stash
#define ENABLE_GUI false
#else
#define ENABLE_GUI true
#endif

#ifndef DATA_PATH
#define DATA_PATH "/opt/cathook"
#endif

#endif /* MACROS_HPP_ */
