#[macro_export]
macro_rules! assign_resources {
    {
        $(
            $(#[$outer:meta])*
            $group_name:ident : $group_struct:ident {
                $(
                    $(#[$inner:meta])*
                    $resource_name:ident : $resource_field:ident $(=$resource_alias:ident)?),*
                $(,)?
            }
            $(,)?
        )+
    } => {
        #[allow(dead_code,non_snake_case,missing_docs)]
        pub struct AssignedResources {
            $(pub $group_name : $group_struct),*
        }
        $(
            #[allow(dead_code,non_snake_case)]
            $(#[$outer])*
            pub struct $group_struct {
                $(
                    $(#[$inner])*
                    pub $resource_name: peripherals::$resource_field
                ),*
            }
        )+


        $($($(
            #[allow(missing_docs)]
            pub type $resource_alias = peripherals::$resource_field;
        )?)*)*

        #[macro_export]
        /// `split_resources!` macro
        macro_rules! split_resources (
            ($p:ident) => {
                AssignedResources {
                    $($group_name: $group_struct {
                        $($resource_name: $p.$resource_field),*
                    }),*
                }
            }
        );
    }
}
