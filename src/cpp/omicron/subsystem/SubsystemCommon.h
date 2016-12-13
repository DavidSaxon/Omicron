#ifndef OMICRON_SUBSYSTEM_COMMON_H_
#define OMICRON_SUBSYSTEM_COMMON_H_

/*!
 * \brief The possible roles a subsystem can fulfill.
 *
 * A subsystem may fulfill one or more of these rolls.
 */
enum ossRole
{
    /// The subsystem does not fulfill a role.
    OSS_ROLE_NONE           = 0L,
    /// The subsystem will provide the window manager functionality, this
    /// includes providing the main window of Omicron, and a GL context.
    OSS_ROLE_WINDOW_MANAGER = 1L << 0,
    /// The subsystem will provide functionality for querying user input through
    /// one or more difference devices.
    OSS_ROLE_INPUT          = 1L << 1,
    /// The subsystem will provide UI rendering and functionality.
    OSS_ROLE_UI             = 1L << 2,
    /// The subsystem will provide Omicron's 3D rendering.
    OSS_ROLE_RENDERER       = 1L << 3,
    /// The subsystem will provide Omicron's physics simulation systems.
    OSS_ROLE_PHYSICS        = 1L << 4,
    /// The subsystem will provide audio playback.
    OSS_ROLE_AUDIO          = 1L << 5
};

#endif
