/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_RENDER_SUBSYSTEM_BOOTSTRAP_HPP_
#define OMICRON_API_RENDER_SUBSYSTEM_BOOTSTRAP_HPP_

namespace omi
{
namespace render
{
namespace ss
{

/*!
 * \brief Object used to bootstrap a rendering subsystem.
 */
class Bootstrap
{
public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Bootstrap()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Starts up this Omicron rendering subsystem.
     *
     * Other than the Bootstrap's constructor this will be the first call made
     * to this object, and will only be made once.
     */
    virtual void startup()
    {
        m_active = true;
    }

    /*!
     * \brief Starts up this Omicron rendering subsystem.
     *
     * Other than the Bootstrap's destructor this will be the last call made
     * to this object, and will only be made once.
     */
    virtual void shutdown()
    {
        m_active = false;
    }

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor for rendering subsystem bootstrappers.
     */
    Bootstrap()
        : m_active(false)
    {
    }

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Whether the bootstrapper has been started up correctly and not yet
     *        shutdown.
     */
    bool m_active;
};

} // namespace ss
} // namespace window
} // namespace omi

#endif
