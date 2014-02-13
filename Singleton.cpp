template <typename N>
class Singleton
{
public:
  static Singleton& instance()
  {
    static Singleton instance;
    return instance;
  }
};
