#ifndef OMICRON_RUNTIME_ENGINE_HPP_
#define OMICRON_RUNTIME_ENGINE_HPP_

namespace omi
{

/*!
 * \brief Singleton object that owns and starts the Omicron runtime.
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

};

} // namespace omi

#endif
