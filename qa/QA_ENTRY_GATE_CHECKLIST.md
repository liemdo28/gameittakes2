# QA ENTRY GATE CHECKLIST (PLAYABLE REQUIRED)

## Gate Objective
Cho phép QA bắt đầu test chính thức khi project đạt mức tối thiểu testable.

## Must-Have Artifacts
- [ ] `ShardsOfDawn.uproject` mở được trong UE5
- [ ] Source module compile thành công
- [ ] Default map cấu hình hợp lệ
- [ ] Build hướng dẫn rõ ràng (local)

## Must-Have Runtime Criteria
- [ ] Chạy được vòng gameplay tối thiểu từ start -> interaction -> end
- [ ] Không crash trong 15 phút smoke run
- [ ] Có fail/retry loop
- [ ] Có log/teleremetry event cơ bản

## Multiplayer Minimal Criteria
- [ ] Host/join thành công trong LAN/dev online setup
- [ ] Không desync blocker trong test case chính
- [ ] Reconnect path có mô tả và test case

## QA Process Criteria
- [ ] Severity taxonomy thống nhất
- [ ] Bug template thống nhất
- [ ] Known issues list được cập nhật

## Entry Decision
- [ ] PASS: QA full entry
- [ ] CONDITIONAL: limited QA entry
- [ ] FAIL: quay lại engineering fix
