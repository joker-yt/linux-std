class Cready {
public:
  Cready(){};

  ~Cready() = default;
  Cready(const Cready &other) = default;
  Cready(Cready &&other) = default;
  Cready &operator=(const Cready &other) = default;
  Cready &operator=(Cready &&other) = default;
};
