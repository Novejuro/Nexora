namespace Nexora::Core {
    class MessageBox {
    public:
        enum class Type {
            Error,
            Warning,
            Info
        };

        enum class Buttons {
            OK,
            YesNo,
            OKCancel
        };

        static void Show(
            const char* title,
            const char* message,
            Type type
        );

        static void ShowConfirm(
            const char* title,
            const char* message
        );
    };
} // namespace Nexora::Core