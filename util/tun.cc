#include "tun.hh"
#include "exception.hh"

#include <cstring>
#include <fcntl.h>
#ifdef __linux__
#include <linux/if.h>
#include <linux/if_tun.h>
#endif
#include <sys/ioctl.h>

#ifdef __linux__
static constexpr const char* CLONEDEV = "/dev/net/tun";
#endif

using namespace std;

//! \param[in] devname is the name of the TUN or TAP device, specified at its creation.
//! \param[in] is_tun is `true` for a TUN device (expects IP datagrams), or `false` for a TAP device (expects
//! Ethernet frames)
//!
//! To create a TUN device, you should already have run
//!
//!     ip tuntap add mode tun user `username` name `devname`
//!
//! as root before calling this function.

#ifdef __linux__
TunTapFD::TunTapFD( const string& devname, const bool is_tun )
  : FileDescriptor( ::CheckSystemCall( "open", open( CLONEDEV, O_RDWR | O_CLOEXEC ) ) )
{
  struct ifreq tun_req {};

  tun_req.ifr_flags = static_cast<int16_t>( ( is_tun ? IFF_TUN : IFF_TAP ) | IFF_NO_PI ); // no packetinfo

  // copy devname to ifr_name, making sure to null terminate

  strncpy( static_cast<char*>( tun_req.ifr_name ), devname.data(), IFNAMSIZ - 1 );
  tun_req.ifr_name[IFNAMSIZ - 1] = '\0';

  CheckSystemCall( "ioctl", ioctl( fd_num(), TUNSETIFF, static_cast<void*>( &tun_req ) ) );
}
#else
TunTapFD::TunTapFD( const string& /*devname*/, const bool /*is_tun*/ )
  : FileDescriptor( -1 )
{
  throw std::runtime_error( "TUN/TAP devices are only supported on Linux" );
}
#endif
