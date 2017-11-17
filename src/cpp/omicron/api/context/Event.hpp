/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_CONTEXT_EVENT_HPP_
#define OMICRON_API_CONTEXT_EVENT_HPP_

#include <arcanecore/base/str/UTF8String.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/common/Attributes.hpp"


namespace omi
{
namespace context
{

/*!
 * \brief Simple object that describes an event that can be propagated through
 *        Omicron.
 */
class Event
{
public:

    //--------------------------------------------------------------------------
    //                                DEFINITIONS
    //--------------------------------------------------------------------------

    //--------------------------E V E N T    N A M E S--------------------------

    /*!
     * \brief The type of mouse move events.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kTypeMouseMove;
    /*!
     * \brief The type of mouse button press events.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kTypeMouseButtonPress;
    /*!
     * \brief The type of mouse button release events.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kTypeMouseButtonRelease;
    /*!
     * \brief The type of mouse scroll events.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kTypeMouseScroll;
    /*!
     * \brief The type of mouse enter events.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kTypeMouseEnter;
    /*!
     * \brief The type of mouse exit events.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kTypeMouseExit;

    /*!
     * \brief The type of key press events.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kTypeKeyPress;
    /*!
     * \brief The type of key release events.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kTypeKeyRelease;

    /*!
     * \brief The type of window resize events.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kTypeWindowResize;
    /*!
     * \brief The type of window move events.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kTypeWindowMove;

    /*!
     * \brief The type of the event which is fired to cause the engine to
     *        shutdown.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kTypeEngineShutdown;

    //---------------------------D A T A    N A M E S---------------------------

    /*!
     * \brief The type of the data attribute that stores the modifiers bit flag.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kDataModifiers;

    /*!
     * \brief The type of the data attribute that stores the mouse position.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kDataMousePosition;
    /*!
     * \brief The type of the data attribute that stores the mouse button.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kDataMouseButton;
    /*!
     * \brief The type of the data attribute that the stores the mouse x scroll
     *        amount.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kDataMouseScrollAmountX;
    /*!
     * \brief The type of the data attribute that the stores the mouse y scroll
     *        amount.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kDataMouseScrollAmountY;

    /*!
     * \brief The type of the data attribute that stores the key code.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kDataKeyCode;

    /*!
     * \brief The type of the data attribute that stores the window size.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kDataWindowSize;
    /*!
     * \brief The type of the data attribute that stores the window position.
     */
    OMI_API_EXPORT static const arc::str::UTF8String kDataWindowPosition;

    //--------------------------D A T A    V A L U E S--------------------------

    /*!
     * \brief The modifier bit flags.
     */
    enum Modifier
    {
        kModifierShift   = 1UL << 0,
        kModifierControl = 1UL << 1,
        kModifierAlt     = 1UL << 2,
        kModifierSuper   = 1UL << 3
    };

    /*!
     * \brief THe various mouse buttons.
     */
    enum MouseButton
    {
        kMouse1      = 0,
        kMouse2      = 1,
        kMouse3      = 2,
        kMouse4      = 3,
        kMouse5      = 4,
        kMouse6      = 5,
        kMouse7      = 6,
        kMouse8      = 7,
        kMouseLeft   = kMouse1,
        kMouseRight  = kMouse2,
        kMouseMiddle = kMouse3,
    };

    /*!
     * \brief The various key codes.
     */
    enum KeyCode
    {
        kKeyCodeUnkown         = -1,
        kKeyCodeSpace          = 32,
        kKeyCodeApostrophe     = 39,
        kKeyCodeComma          = 44,
        kKeyCodeMinus          = 45,
        kKeyCodePeriod         = 46,
        kKeyCodeSlash          = 47,
        kKeyCode0              = 48,
        kKeyCode1              = 49,
        kKeyCode2              = 50,
        kKeyCode3              = 51,
        kKeyCode4              = 52,
        kKeyCode5              = 53,
        kKeyCode6              = 54,
        kKeyCode7              = 55,
        kKeyCode8              = 56,
        kKeyCode9              = 57,
        kKeyCodeSemicolon      = 59,
        kKeyCodeEqual          = 61,
        kKeyCodeA              = 65,
        kKeyCodeB              = 66,
        kKeyCodeC              = 67,
        kKeyCodeD              = 68,
        kKeyCodeE              = 69,
        kKeyCodeF              = 70,
        kKeyCodeG              = 71,
        kKeyCodeH              = 72,
        kKeyCodeI              = 73,
        kKeyCodeJ              = 74,
        kKeyCodeK              = 75,
        kKeyCodeL              = 76,
        kKeyCodeM              = 77,
        kKeyCodeN              = 78,
        kKeyCodeO              = 79,
        kKeyCodeP              = 80,
        kKeyCodeQ              = 81,
        kKeyCodeR              = 82,
        kKeyCodeS              = 83,
        kKeyCodeT              = 84,
        kKeyCodeU              = 85,
        kKeyCodeV              = 86,
        kKeyCodeW              = 87,
        kKeyCodeX              = 88,
        kKeyCodeY              = 89,
        kKeyCodeZ              = 90,
        kKeyCodeLeftBracket    = 91,
        kKeyCodeBackslash      = 92,
        kKeyCodeRightBracket   = 93,
        kKeyCodeGraveAccent    = 96,
        kKeyCodeWorld1         = 161,
        kKeyCodeWorld2         = 162,
        kKeyCodeEscape         = 256,
        kKeyCodeEnter          = 257,
        kKeyCodeTab            = 258,
        kKeyCodeBackspace      = 259,
        kKeyCodeInsert         = 260,
        kKeyCodeDelete         = 261,
        kKeyCodeRight          = 262,
        kKeyCodeLeft           = 263,
        kKeyCodeDown           = 264,
        kKeyCodeUp             = 265,
        kKeyCodePageUp         = 266,
        kKeyCodePageDown       = 267,
        kKeyCodeHome           = 268,
        kKeyCodeEnd            = 269,
        kKeyCodeCapsLock       = 280,
        kKeyCodeScrollLock     = 281,
        kKeyCodeNumLock        = 282,
        kKeyCodePrintScreen    = 283,
        kKeyCodePause          = 284,
        kKeyCodeF1             = 290,
        kKeyCodeF2             = 291,
        kKeyCodeF3             = 292,
        kKeyCodeF4             = 293,
        kKeyCodeF5             = 294,
        kKeyCodeF6             = 295,
        kKeyCodeF7             = 296,
        kKeyCodeF8             = 297,
        kKeyCodeF9             = 298,
        kKeyCodeF10            = 299,
        kKeyCodeF11            = 300,
        kKeyCodeF12            = 301,
        kKeyCodeF13            = 302,
        kKeyCodeF14            = 303,
        kKeyCodeF15            = 304,
        kKeyCodeF16            = 305,
        kKeyCodeF17            = 306,
        kKeyCodeF18            = 307,
        kKeyCodeF19            = 308,
        kKeyCodeF20            = 309,
        kKeyCodeF21            = 310,
        kKeyCodeF22            = 311,
        kKeyCodeF23            = 312,
        kKeyCodeF24            = 313,
        kKeyCodeF25            = 314,
        kKeyCodeKeyPad0        = 320,
        kKeyCodeKeyPad1        = 321,
        kKeyCodeKeyPad2        = 322,
        kKeyCodeKeyPad3        = 323,
        kKeyCodeKeyPad4        = 324,
        kKeyCodeKeyPad5        = 325,
        kKeyCodeKeyPad6        = 326,
        kKeyCodeKeyPad7        = 327,
        kKeyCodeKeyPad8        = 328,
        kKeyCodeKeyPad9        = 329,
        kKeyCodeKeyPadDecimal  = 330,
        kKeyCodeKeyPadDivide   = 331,
        kKeyCodeKeyPadMultiply = 332,
        kKeyCodeKeyPadSubtract = 333,
        kKeyCodeKeyPadAdd      = 334,
        kKeyCodeKeyPadEnter    = 335,
        kKeyCodeKeyPadEqual    = 336,
        kKeyCodeLeftShift      = 340,
        kKeyCodeLeftControl    = 341,
        kKeyCodeLeftAlt        = 342,
        kKeyCodeLeftSuper      = 343,
        kKeyCodeRightShift     = 344,
        kKeyCodeRightControl   = 345,
        kKeyCodeRightAlt       = 346,
        kKeyCodeRightSuper     = 347,
        kKeyCodeMenu           = 348
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new event with a type and a map of data describing
     *        the event.
     */
    OMI_API_EXPORT Event(
            const arc::str::UTF8String& type,
            const omi::MapAttribute& data);

    /*!
     * \brief Copy constructor.
     */
    OMI_API_EXPORT Event(const Event& other);

    /*!
     * \brief Move constructor.
     */
    OMI_API_EXPORT Event(Event&& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~Event();

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Assignment operator.
     */
    OMI_API_EXPORT Event& operator=(const Event& other);

    /*!
     * \brief Move assignment operator.
     */
    OMI_API_EXPORT Event& operator=(Event&& other);

    /*!
     * \brief Equality operator.
     */
    OMI_API_EXPORT bool operator==(const Event& other) const;

    /*!
     * \brief Inequality operator.
     */
    OMI_API_EXPORT bool operator!=(const Event& other) const;

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        mouse move event.
     *
     * If the event is a valid mouse move, the x and y position are returned
     * via the parameters.
     */
    OMI_API_EXPORT static bool mouse_move(
            const omi::context::Event& event,
            arc::int32& position_x,
            arc::int32& position_y);

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        mouse button press event.
     *
     * If the event is a mouse button press, the button is returned via the
     *  parameter.
     */
    OMI_API_EXPORT static bool mouse_button_press(
            const omi::context::Event& event,
            omi::context::Event::MouseButton& button);

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        key press event.
     *
     * If the event is a valid mouse button press, the button and modifiers are
     * returned via the parameters.
     */
    OMI_API_EXPORT static bool mouse_button_press(
            const omi::context::Event& event,
            omi::context::Event::MouseButton& button,
            omi::context::Event::Modifier& modifiers);

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        mouse button release event.
     *
     * If the event is a mouse button release, the button is returned via the
     *  parameter.
     */
    OMI_API_EXPORT static bool mouse_button_release(
            const omi::context::Event& event,
            omi::context::Event::MouseButton& button);

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        key release event.
     *
     * If the event is a valid mouse button release, the button and modifiers
     * are returned via the parameters.
     */
    OMI_API_EXPORT static bool mouse_button_release(
            const omi::context::Event& event,
            omi::context::Event::MouseButton& button,
            omi::context::Event::Modifier& modifiers);

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        mouse scroll event.
     *
     * If the event is a valid mouse scroll, the x and y amounts are returned
     * via the parameters.
     */
    OMI_API_EXPORT static bool mouse_scroll(
            const omi::context::Event& event,
            arc::int32& amount_x,
            arc::int32& amount_y);

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        key press event.
     *
     * If the event is a valid key press, the KeyCode is returned via the
     * parameter.
     */
    OMI_API_EXPORT static bool key_press(
            const omi::context::Event& event,
            omi::context::Event::KeyCode& key_code);

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        key press event.
     *
     * If the event is a valid key press, the KeyCode and modifiers are returned
     * via the parameters.
     */
    OMI_API_EXPORT static bool key_press(
            const omi::context::Event& event,
            omi::context::Event::KeyCode& key_code,
            omi::context::Event::Modifier& modifiers);

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        key release event.
     *
     * If the event is a valid key release, the KeyCode is returned via the
     * parameter.
     */
    OMI_API_EXPORT static bool key_release(
            const omi::context::Event& event,
            omi::context::Event::KeyCode& key_code);

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        key release event.
     *
     * If the event is a valid key release, the KeyCode and modifiers are
     * returned via the parameters.
     */
    OMI_API_EXPORT static bool key_release(
            const omi::context::Event& event,
            omi::context::Event::KeyCode& key_code,
            omi::context::Event::Modifier& modifiers);

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        window resize event.
     *
     * If the event is a valid window resize, the new width and height are
     * returned via the parameters.
     */
    OMI_API_EXPORT static bool window_resize(
            const omi::context::Event& event,
            arc::int32& width,
            arc::int32& height);

    /*!
     * \brief Utility function that returns true if the given event is a valid
     *        window move event.
     *
     * If the event is a valid window move, the new x and y positions are
     * returned via the parameters.
     */
    OMI_API_EXPORT static bool window_move(
            const omi::context::Event& event,
            arc::int32& position_x,
            arc::int32& position_y);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the type of this event.
     */
    OMI_API_EXPORT const arc::str::UTF8String& get_type() const;

    /*!
     * \brief Returns the data describing this event.
     */
    OMI_API_EXPORT const omi::MapAttribute& get_data() const;

private:

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    arc::str::UTF8String m_type;
    MapAttribute m_data;
};

} // namespace context
} // namespace omi

#endif
