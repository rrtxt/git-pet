class Status {
public:
  int staged() const;

  int modified() const;

  int untracked() const;

  bool clean() const;
};
