#ifndef OMICRON_ENGINE_HPP_
#define OMICRON_ENGINE_HPP_

namespace omi
{

/*!
 * \brief Singleton object that provides access to the components of the Omicron
 *        engine.
 */
class Engine
{
public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~Engine();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the Omicron engine.
     */
    static Engine* get_instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Begins execution of Omicron.
     *
     *  Control is only returned from this function once Omicron exits.
     */
    int execute();

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    Engine();

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Performs the initialisation routines of the Omicron.
     */
    void initialisation_routines();

};

} // namespace omi

#endif
