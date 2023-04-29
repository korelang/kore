namespace kore {
    namespace kir {
        /// Register states in KIR
        enum class RegisterState {
            // Register is available for use
            Available = 0,

            // Register was moved
            Moved,

            // Register was maybe moved (e.g. in one branch of a conditional)
            MaybeMoved
        };
    }
}
