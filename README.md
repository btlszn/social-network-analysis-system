# Sosyal Ağ Analizi ve İlişki Ağacı

Bu proje, sosyal ağ yapılarını analiz etmek ve kullanıcılar arasındaki ilişkileri görselleştirmek için geliştirilmiş kapsamlı bir C programıdır. Kırmızı-Siyah Ağaç (Red-Black Tree) veri yapısını kullanarak verimli kullanıcı arama işlemleri sağlar ve çeşitli sosyal ağ metriklerini hesaplar.

## Özellikler

- **Kullanıcı Yönetimi**: Ağa kolayca kullanıcı ekleme
- **İlişki Ağacı**: Kırmızı-Siyah Ağaç kullanarak hızlı kullanıcı arama
- **Arkadaşlık Bağlantıları**: Kullanıcılar arasında çift yönlü bağlantı oluşturma
- **Sosyal Ağ Analizleri**:
  - Belirli mesafedeki arkadaşları bulma (BFS algoritması ile)
  - İki kullanıcı arasındaki ortak arkadaşları tespit etme
  - Topluluk tespiti (Union-Find algoritması kullanarak)
  - Etki alanı hesaplama (kullanıcıların ağ üzerindeki etkisi)
- **Kullanıcı Dostu Arayüz**: Komut satırı tabanlı interaktif menüler

## Teknik Detaylar

### Veri Yapıları

- **User**: Kullanıcı bilgilerini ve bağlantılarını tutan yapı
- **RBNode**: Kırmızı-Siyah Ağaç düğümü
- **Graph**: Tüm kullanıcıları ve ağaç yapısını içeren ana veri yapısı
- **Stack**: DFS için yığın yapısı
- **Queue**: BFS için kuyruk yapısı
- **VisitedTracker**: Ziyaret edilmiş düğümleri takip etmek için yapı

### Algoritmalar

- **Kırmızı-Siyah Ağaç**: Dengeli bir ikili arama ağacı implementasyonu
- **BFS (Breadth-First Search)**: Belirli mesafedeki arkadaşları bulmak için
- **Union-Find**: Topluluk tespiti için
- **Etki Hesaplama**: Doğrudan ve dolaylı bağlantılara dayalı etki puanı hesaplama

## Kurulum

### Gereksinimler

- C Derleyicisi (GCC önerilir)
- Standard C Kütüphanesi

## Kullanım Kılavuzu

### Ana Menü

Program çalıştırıldığında, aşağıdaki ana menü görüntülenir:

```
--- Ana Menü ---
1. Kullanıcı ekle
2. Arkadaşlık bağlantıları ekle
3. Analiz yap
4. Çıkış
```

### Kullanıcı Ekleme

"Kullanıcı ekle" seçeneği ile ağa yeni kullanıcılar ekleyebilirsiniz. Her kullanıcı için bir isim girmeniz istenecektir.

### Arkadaşlık Bağlantıları Ekleme

"Arkadaşlık bağlantıları ekle" seçeneği ile kullanıcılar arasında bağlantılar oluşturabilirsiniz. İki kullanıcının ID'lerini girerek onları arkadaş olarak tanımlayabilirsiniz.

### Analiz İşlemleri

"Analiz yap" seçeneği ile çeşitli sosyal ağ analizlerini gerçekleştirebilirsiniz:

1. **Belirli mesafedeki arkadaşları bul**: Bir kullanıcıdan belirli bir mesafedeki tüm kullanıcıları listeler
2. **Ortak arkadaşları bul**: İki kullanıcı arasındaki ortak arkadaşları gösterir
3. **Toplulukları tespit et**: Ağdaki bağlantılı toplulukları tespit eder ve listeler
4. **Etki alanını hesapla**: Kullanıcıların ağ üzerindeki etkilerini hesaplar ve en etkili kullanıcıları sıralar
5. **Kullanıcı bilgilerini göster**: Bir kullanıcının tüm detaylarını ve bağlantılarını görüntüler
