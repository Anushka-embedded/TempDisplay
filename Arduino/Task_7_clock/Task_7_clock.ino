
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buttons
const int SELECT_BTN = 34;
const int UP_BTN     = 35;
const int DOWN_BTN   = 36;
const int BACK_BTN   = 39;

const int pins[] = {SELECT_BTN, UP_BTN, DOWN_BTN, BACK_BTN};

enum Screen { HOME, MENU, EDIT_TIME, EDIT_DATE };
Screen screen = HOME;

const char* menuItems[] = {"SET TIME", "SET DATE", "SET CLOCK"};
int menuIndex = 0;

// Initial time and date; edit these from the menu
int hh = 10, mm = 30, ss = 0;
int dd = 27, mon = 10, yyyy = 2026;

int eh, em, es, ed, eMon, eYear;
int field = 0;

unsigned long lastTick = 0;
unsigned long lastDraw = 0;

bool lastRead[4] = {HIGH, HIGH, HIGH, HIGH};
bool stableRead[4] = {HIGH, HIGH, HIGH, HIGH};
unsigned long changedAt[4] = {0, 0, 0, 0};

bool pressed(int i) {
  bool r = digitalRead(pins[i]);

  if (r != lastRead[i]) {
    lastRead[i] = r;
    changedAt[i] = millis();
  }

  if (millis() - changedAt[i] > 35 && r != stableRead[i]) {
    stableRead[i] = r;
    if (r == LOW) return true;
  }
  return false;
}

bool leap(int y) {
  return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

int monthDays(int m, int y) {
  const int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  if (m == 2 && leap(y)) return 29;
  return days[m - 1];
}

void tickClock() {
  if (++ss >= 60) {
    ss = 0;
    if (++mm >= 60) {
      mm = 0;
      if (++hh >= 24) {
        hh = 0;
        if (++dd > monthDays(mon, yyyy)) {
          dd = 1;
          if (++mon > 12) {
            mon = 1;
            yyyy++;
          }
        }
      }
    }
  }
}

void twoDigits(int n) {
  if (n < 10) lcd.print('0');
  lcd.print(n);
}

void showHome() {
  lcd.setCursor(0, 0);
  lcd.print("TIME ");
  twoDigits(hh); lcd.print(':');
  twoDigits(mm); lcd.print(':');
  twoDigits(ss); lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("DATE ");
  twoDigits(dd); lcd.print('/');
  twoDigits(mon); lcd.print('/');
  lcd.print(yyyy);
}

void showMenu() {
  lcd.setCursor(0, 0);
  lcd.print("   MAIN MENU   ");
  lcd.setCursor(0, 1);
  lcd.print(menuIndex + 1);
  lcd.print(": ");
  lcd.print(menuItems[menuIndex]);
  int n = 3 + strlen(menuItems[menuIndex]);
  while (n++ < 16) lcd.print(' ');
}

void showTimeEdit() {
  lcd.setCursor(0, 0);
  lcd.print("SET TIME        ");
  lcd.setCursor(0, 1);

  lcd.print(field == 0 ? '>' : ' ');
  twoDigits(eh); lcd.print(':');
  lcd.print(field == 1 ? '>' : ' ');
  twoDigits(em); lcd.print(':');
  lcd.print(field == 2 ? '>' : ' ');
  twoDigits(es); lcd.print(' ');
}

void showDateEdit() {
  lcd.setCursor(0, 0);
  lcd.print("SET DATE        ");
  lcd.setCursor(0, 1);

  lcd.print(field == 0 ? '>' : ' ');
  twoDigits(ed); lcd.print('/');
  lcd.print(field == 1 ? '>' : ' ');
  twoDigits(eMon); lcd.print('/');
  lcd.print(field == 2 ? '>' : ' ');
  lcd.print(eYear);
}

void drawScreen() {
  lcd.clear();

  if (screen == HOME) showHome();
  else if (screen == MENU) showMenu();
  else if (screen == EDIT_TIME) showTimeEdit();
  else showDateEdit();
}

void startTimeEdit() {
  eh = hh; em = mm; es = ss;
  field = 0;
  screen = EDIT_TIME;
}

void startDateEdit() {
  ed = dd; eMon = mon; eYear = yyyy;
  field = 0;
  screen = EDIT_DATE;
}

void changeTime(int d) {
  if (field == 0) eh = (eh + d + 24) % 24;
  else if (field == 1) em = (em + d + 60) % 60;
  else es = (es + d + 60) % 60;
}

void changeDate(int d) {
  if (field == 0) {
    ed += d;
    if (ed > monthDays(eMon, eYear)) ed = 1;
    if (ed < 1) ed = monthDays(eMon, eYear);
  } else if (field == 1) {
    eMon += d;
    if (eMon > 12) eMon = 1;
    if (eMon < 1) eMon = 12;
    if (ed > monthDays(eMon, eYear)) ed = monthDays(eMon, eYear);
  } else {
    eYear += d;
    if (eYear > 2099) eYear = 2000;
    if (eYear < 2000) eYear = 2099;
    if (ed > monthDays(eMon, eYear)) ed = monthDays(eMon, eYear);
  }
}

void handleButtons() {
  bool sel = pressed(0);
  bool up = pressed(1);
  bool down = pressed(2);
  bool back = pressed(3);

  if (screen == HOME) {
    if (sel) { menuIndex = 0; screen = MENU; }
    return;
  }

  if (screen == MENU) {
    if (up) menuIndex = (menuIndex + 2) % 3;
    if (down) menuIndex = (menuIndex + 1) % 3;
    if (back) screen = HOME;

    if (sel) {
      if (menuIndex == 0) startTimeEdit();
      else if (menuIndex == 1) startDateEdit();
      else {
        ss = 0;
        lastTick = millis();
        screen = HOME;
      }
    }
    return;
  }

  if (screen == EDIT_TIME) {
    if (up) changeTime(1);
    if (down) changeTime(-1);
    if (back) screen = MENU;

    if (sel && screen == EDIT_TIME) {
      if (++field > 2) {
        hh = eh; mm = em; ss = es;
        lastTick = millis();
        screen = HOME;
      }
    }
    return;
  }

  if (screen == EDIT_DATE) {
    if (up) changeDate(1);
    if (down) changeDate(-1);
    if (back) screen = MENU;

    if (sel && screen == EDIT_DATE) {
      if (++field > 2) {
        dd = ed; mon = eMon; yyyy = eYear;
        screen = HOME;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  for (int i = 0; i < 4; i++) pinMode(pins[i], INPUT);

  lcd.setCursor(0, 0);
  lcd.print("ESP32 CLOCK");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(1000);

  lastTick = millis();
  drawScreen();
}

void loop() {
  unsigned long now = millis();

  while (now - lastTick >= 1000) {
    lastTick += 1000;
    tickClock();
  }

  handleButtons();

  if (millis() - lastDraw >= 250) {
    lastDraw = millis();
    drawScreen();
  }
}


